/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   names.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 17:26:59 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/11 13:14:48 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/Server.hpp"
#include "maximum_length_for_message.hpp"
#include "replies.hpp"
#include "split.hpp"
#include <list>

typedef std::vector<std::string>          ChannelNameVector;
typedef ChannelNameVector::const_iterator ChannelNameIterator;

/**
 * @brief Lists the members of a channel.
 *
 * @param client The client to send back the lists of channel members to.
 * @param channel_name The name of the channel for which the members will be listed.
 * @param channel The channel for which the members will be listed.
 */
inline static void list_channel_members(Client &client, ChannelName const &channel_name, Channel const &channel)
{
	std::string const prefixed_channel_name = "=" + channel_name;
	size_t const maximum_length_for_members_as_string = MAXIMUM_LENGTH_FOR_MESSAGE - prefixed_channel_name.size() - 2;
	std::string  members_as_string = channel.members_as_string();

	while (members_as_string.size() > maximum_length_for_members_as_string)
	{
		size_t const      space_pos = members_as_string.rfind(' ', maximum_length_for_members_as_string - 1);
		std::string const members_as_string_part = members_as_string.substr(0, space_pos);

		client.append_formatted_reply_to_msg_out(RPL_NAMESREPLY, &prefixed_channel_name, &members_as_string_part);
		members_as_string.erase(0, space_pos + 1);
	}
	client.append_formatted_reply_to_msg_out(RPL_NAMESREPLY, &prefixed_channel_name, &members_as_string);
	client.append_formatted_reply_to_msg_out(RPL_ENDOFNAMES, &channel_name);
}

/**
 * @brief Lists the members of the channels joined by a user.
 *
 * @param client The user for which the channel members of their joined channels will be listed.
 */
inline static void list_members_of_joined_channels(Client &client)
{
	Client::JoinedChannelMap const     &joined_channels_by_name = client.get_joined_channels_by_name();
	Client::JoinedChannelIterator const end = joined_channels_by_name.end();

	for (Client::JoinedChannelIterator cit = joined_channels_by_name.begin(); cit != end; ++cit)
		list_channel_members(client, cit->first, *cit->second);
}

/**
 * @brief Lists the members of specific channels.
 *
 * @param client The client to send back the lists of channel members to.
 * @param channel_names The names of the channels for which the members will be listed.
 */
inline static void list_members_of_specific_channels(
	Client                   &client,
	ChannelNameVector const  &channel_names,
	Server::ChannelMap const &channels_by_name)
{
	ChannelNameIterator const end = channel_names.end();

	for (ChannelNameIterator cit = channel_names.begin(); cit != end; ++cit)
	{
		ChannelName const                 &channel_name = *cit;
		Server::ChannelConstIterator const channel_by_name = channels_by_name.find(channel_name);

		if (channel_by_name != channels_by_name.end())
			list_channel_members(client, channel_name, channel_by_name->second);
	}
}

/**
 * @brief Lists either the members of specific channels or the members of the channels joined by a user.
 *
 * @param sender The client that sent the command.
 * @param parameters The parameters of the command.
 *
 * @throw `UnknownReply` if a given reply number isn't recognized.
 * @throw `InvalidConversion` if a conversion specification is invalid.
 * @throw `std::exception` if a function of the C++ standard library critically fails.
 */
void Server::_names(Client &sender, std::vector<std::string> const &parameters)
{
	if (!sender.has_mode(AlreadySentUser))
		return sender.append_formatted_reply_to_msg_out(ERR_NOTREGISTERED);

	if (parameters.empty())
		return list_members_of_joined_channels(sender);
	list_members_of_specific_channels(sender, split<ChannelNameVector>(parameters[0], ','), this->_channels_by_name);
}
