/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 17:27:38 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/15 06:03:47 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/Server.hpp"
#include "replies.hpp"
#include "split.hpp"
#include <list>

#define DEFAULT_KICK_TEXT "Move, bitch, get out da way!"

typedef std::list<ChannelName> ChannelNameList;
typedef std::list<NickName>    NickNameList;

// TODO: write the doxygen comment for this function
inline static void kick_several_users_from_one_channel(
	Server::ClientMap const &users_by_nickname,
	Server::ChannelMap      &channels_by_name,
	Client                  &sender,
	ChannelName const       &channel_name,
	NickNameList const      &nicknames,
	std::string const       &comment)
{
	Server::ChannelMap::iterator const channel_by_name = channels_by_name.find(channel_name);

	if (channel_by_name == channels_by_name.end())
		return sender.append_formatted_reply_to_msg_out(ERR_NOSUCHCHANNEL, &channel_name);

	Channel &channel = channel_by_name->second;

	if (!channel.get_modes().has_operator(sender))
		return sender.append_formatted_reply_to_msg_out(ERR_CHANOPRIVSNEEDED, &channel_name);

	for (NickNameList::const_iterator nickname = nicknames.begin(); nickname != nicknames.end(); ++nickname)
	{
		Server::ClientMap::const_iterator const user_by_nickname = users_by_nickname.find(*nickname);

		if (user_by_nickname == users_by_nickname.end())
		{
			sender.append_formatted_reply_to_msg_out(ERR_USERNOTONCHANNEL, &*nickname, &channel_name);
			continue;
		}

		Client &target = *user_by_nickname->second;

		if (channel.has_member(target))
			make_user_leave_channel(
				channels_by_name,
				target,
				channel_name,
				sender.prefix() + "KICK " + channel_name + ' ' + *nickname + " :" + comment);
		else
			sender.append_formatted_reply_to_msg_out(ERR_USERNOTONCHANNEL, &*nickname, &channel_name);
	}
}

// TODO: write the doxygen comment for this function
inline static void kick_several_user_from_several_channels(
	Server::ClientMap const &users_by_nickname,
	Server::ChannelMap      &channels_by_name,
	Client                  &sender,
	ChannelNameList const   &channel_names,
	NickNameList const      &nicknames,
	std::string const       &comment)
{
	ChannelNameList::const_iterator channel_name = channel_names.begin();
	NickNameList::const_iterator    nickname = nicknames.begin();

	for (; channel_name != channel_names.end(); ++channel_name, ++nickname)
	{
		Server::ChannelMap::iterator const channel_by_name = channels_by_name.find(*channel_name);

		if (channel_by_name == channels_by_name.end())
			sender.append_formatted_reply_to_msg_out(ERR_NOSUCHCHANNEL, &*channel_name);
		else
		{
			Server::ClientMap::const_iterator const user_by_nickname = users_by_nickname.find(*nickname);

			if (user_by_nickname == users_by_nickname.end())
			{
				sender.append_formatted_reply_to_msg_out(ERR_USERNOTONCHANNEL, &*nickname, &*channel_name);
				continue;
			}

			Channel &channel = channel_by_name->second;
			Client  &target = *user_by_nickname->second;

			if (!channel.get_modes().has_operator(sender))
				sender.append_formatted_reply_to_msg_out(ERR_CHANOPRIVSNEEDED, &*channel_name);
			else if (!channel.has_member(target))
				sender.append_formatted_reply_to_msg_out(ERR_USERNOTONCHANNEL, &*nickname, &*channel_name);
			else
				make_user_leave_channel(
					channels_by_name,
					target,
					*channel_name,
					sender.prefix() + "KICK " + *channel_name + ' ' + *nickname + " :" + comment);
		}
	}
}

// TODO: write the doxygen comment for this function
inline static void kick_one_client_from_several_channels(
	Server::ClientMap const &users_by_nickname,
	Server::ChannelMap      &channels_by_name,
	Client                  &sender,
	ChannelNameList const   &channel_names,
	NickName const          &nickname,
	std::string const       &comment)
{
	Server::ClientMap::const_iterator const user_by_nickname = users_by_nickname.find(nickname);

	if (user_by_nickname == users_by_nickname.end())
	{
		for (ChannelNameList::const_iterator channel_name = channel_names.begin(); channel_name != channel_names.end();
		     ++channel_name)
			sender.append_formatted_reply_to_msg_out(ERR_USERNOTONCHANNEL, &nickname, &*channel_name);
		return;
	}

	Client &target = *user_by_nickname->second;

	for (ChannelNameList::const_iterator channel_name = channel_names.begin(); channel_name != channel_names.end();
	     ++channel_name)
	{
		Server::ChannelMap::iterator const channel_by_name = channels_by_name.find(*channel_name);

		if (channel_by_name == channels_by_name.end())
		{
			sender.append_formatted_reply_to_msg_out(ERR_NOSUCHCHANNEL, &*channel_name);
			continue;
		}

		Channel &channel = channel_by_name->second;

		if (!channel.get_modes().has_operator(sender))
			sender.append_formatted_reply_to_msg_out(ERR_CHANOPRIVSNEEDED, &*channel_name);
		else if (!channel.has_member(target))
			sender.append_formatted_reply_to_msg_out(ERR_USERNOTONCHANNEL, &nickname, &*channel_name);
		else
			make_user_leave_channel(
				channels_by_name,
				target,
				*channel_name,
				sender.prefix() + "KICK " + *channel_name + ' ' + nickname + " :" + comment);
	}
}

/**
 * @brief Makes a channel operator force a channel member to leave the channel.
 *
 * @param sender The client that sent the command.
 * @param parameters The parameters of the command.
 *
 * @throw `UnknownReply` if a given reply number isn't recognized.
 * @throw `InvalidConversion` if a conversion specification is invalid.
 * @throw `std::exception` if a function of the C++ standard library critically fails.
 */
void Server::_kick(Client &sender, CommandParameterVector const &parameters)
{
	if (!sender.is_registered())
		return sender.append_formatted_reply_to_msg_out(ERR_NOTREGISTERED);

	size_t const parameters_len = parameters.size();

	if (parameters_len < 2)
		return sender.append_formatted_reply_to_msg_out(ERR_NEEDMOREPARAMS, "KICK");

	ChannelNameList const channel_names = split<ChannelNameList>(parameters[0], ',');
	size_t const          channel_names_len = channel_names.size();
	NickNameList const    nicknames = split<NickNameList>(parameters[1], ',');
	size_t const          nicknames_len = nicknames.size();

	if (channel_names_len == 0 || nicknames_len == 0)
		return sender.append_formatted_reply_to_msg_out(ERR_NEEDMOREPARAMS, "KICK");

	if (channel_names_len > 1 && nicknames_len > 1)
	{
		if (channel_names_len != nicknames_len)
			return sender.append_formatted_reply_to_msg_out(ERR_NEEDMOREPARAMS, "KICK");

		return kick_several_user_from_several_channels(
			this->_clients_by_nickname,
			this->_channels_by_name,
			sender,
			channel_names,
			nicknames,
			parameters_len > 2 ? parameters[2] : DEFAULT_KICK_TEXT);
	}
	if (channel_names_len == 1)
		return kick_several_users_from_one_channel(
			this->_clients_by_nickname,
			this->_channels_by_name,
			sender,
			channel_names.front(),
			nicknames,
			parameters_len > 2 ? parameters[2] : DEFAULT_KICK_TEXT);
	return kick_one_client_from_several_channels(
		this->_clients_by_nickname,
		this->_channels_by_name,
		sender,
		channel_names,
		nicknames.front(),
		parameters_len > 2 ? parameters[2] : DEFAULT_KICK_TEXT);
}
