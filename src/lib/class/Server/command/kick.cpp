/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 17:27:38 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/12 00:25:51 by mcourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/Server.hpp"
#include "replies.hpp"
#include "split.hpp"

#define DEFAULT_KICK_MSG "You have been kicked from the channel"

typedef std::vector<ChannelName> ChannelNameVector;
typedef std::vector<NickName>    NickNameVector;

inline static void kick_user(
	Client const      &sender,
	ChannelName const &channel_name,
	Channel           &channel,
	NickName const    &target_nickname,
	Client            &target,
	std::string const &comment)
{
	channel.broadcast_to_all_members(sender.prefix() + "KICK " + channel_name + ' ' + target_nickname + " :" + comment);
	target.leave_channel(channel_name);
	channel.remove_member(target);
}

inline static void kick_from_one_channel(
	Client                                  &sender,
	Server::ChannelMap                      &channels_by_name,
	std::map<NickName, Client *const> const &users_by_nickname,
	ChannelName const                       &channel_name,
	NickNameVector const                    &nicknames,
	std::string const                       &comment)
{
	Server::ChannelIterator const channel_by_name = channels_by_name.find(channel_name);

	if (channel_by_name == channels_by_name.end())
		return sender.append_formatted_reply_to_msg_out(ERR_NOSUCHCHANNEL, &channel_name);

	Channel &channel = channel_by_name->second;

	if (!channel.get_modes().has_operator(sender))
		return sender.append_formatted_reply_to_msg_out(ERR_CHANOPRIVSNEEDED, &channel_name);

	size_t const nicknames_len = nicknames.size();

	for (size_t i = 0; i < nicknames_len; ++i)
	{
		NickName const                                         &target_nickname = nicknames[i];
		std::map<NickName, Client *const>::const_iterator const user_by_nickname =
			users_by_nickname.find(target_nickname);

		if (user_by_nickname == users_by_nickname.end())
		{
			sender.append_formatted_reply_to_msg_out(ERR_USERNOTONCHANNEL, &target_nickname, &channel_name);
			continue;
		}

		Client &target = *user_by_nickname->second;

		if (!channel.has_member(target))
			sender.append_formatted_reply_to_msg_out(ERR_USERNOTONCHANNEL, &target_nickname, &channel_name);
		else
			kick_user(sender, channel_name, channel, target_nickname, target, comment);
	}
}

inline static void kick_from_several_channels(
	Client                                  &sender,
	Server::ChannelMap                      &channels_by_name,
	std::map<NickName, Client *const> const &users_by_nickname,
	ChannelNameVector const                 &channel_names,
	NickNameVector const                    &nicknames,
	std::string const                       &comment)
{
	size_t const channel_names_len = channel_names.size();

	for (size_t i = 0; i < channel_names_len; ++i)
	{
		ChannelName const            &channel_name = channel_names[i];
		Server::ChannelIterator const channel_by_name = channels_by_name.find(channel_name);

		if (channel_by_name == channels_by_name.end())
			sender.append_formatted_reply_to_msg_out(ERR_NOSUCHCHANNEL, &channel_names[i]);
		else
		{
			NickName const                                         &target_nickname = nicknames[i];
			std::map<NickName, Client *const>::const_iterator const user_by_nickname =
				users_by_nickname.find(target_nickname);

			if (user_by_nickname == users_by_nickname.end())
			{
				sender.append_formatted_reply_to_msg_out(ERR_USERNOTONCHANNEL, &target_nickname, &channel_name);
				continue;
			}

			Channel &channel = channel_by_name->second;
			Client  &target = *user_by_nickname->second;

			if (!channel.get_modes().has_operator(sender))
				sender.append_formatted_reply_to_msg_out(ERR_CHANOPRIVSNEEDED, &channel_name);
			else if (!channel.has_member(target))
				sender.append_formatted_reply_to_msg_out(ERR_USERNOTONCHANNEL, &target_nickname, &channel_name);
			else
				kick_user(sender, channel_name, channel, target_nickname, target, comment);
		}
	}
}

void Server::_kick(Client &sender, std::vector<std::string> const &parameters)
{
	if (!sender.has_mode(AlreadySentUser))
		return sender.append_formatted_reply_to_msg_out(ERR_NOTREGISTERED);

	size_t const parameters_len = parameters.size();

	if (parameters_len < 2)
		return sender.append_formatted_reply_to_msg_out(ERR_NEEDMOREPARAMS, "KICK");

	ChannelNameVector const channel_names = split<ChannelNameVector>(parameters[0], ',');
	size_t const            channel_names_len = channel_names.size();
	NickNameVector const    nicknames = split<NickNameVector>(parameters[1], ',');

	if (channel_names_len != 1 && (nicknames.size() != channel_names_len))
		return sender.append_formatted_reply_to_msg_out(ERR_NEEDMOREPARAMS, "KICK");

	std::string const comment = parameters_len > 2 ? parameters[2] : DEFAULT_KICK_MSG;

	if (channel_names_len == 1)
		return kick_from_one_channel(
			sender,
			this->_channels_by_name,
			this->_clients_by_nickname,
			channel_names[0],
			nicknames,
			comment);
	kick_from_several_channels(
		sender,
		this->_channels_by_name,
		this->_clients_by_nickname,
		channel_names,
		nicknames,
		comment);
}
