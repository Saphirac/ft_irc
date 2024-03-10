/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 17:27:38 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/10 06:18:06 by mcourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/Server.hpp"
#include "replies.hpp"
#include "split.hpp"

typedef std::vector<ChannelName> ChannelNameVector;
typedef std::vector<NickName>    NickNameVector;

inline static void __kick_client(
	Channel           &channel,
	ChannelName const &chan_name,
	NickName const    &user_to_remove_nickname,
	Client            &user_to_remove)
{
	channel.remove_member(user_to_remove);
	user_to_remove.leave_channel(chan_name);
	user_to_remove.append_to_msg_out(user_to_remove.prefix() + "KICK :" + chan_name + " " + user_to_remove_nickname);
	channel.broadcast_to_all_members(user_to_remove.prefix() + "KICK " + chan_name + " " + user_to_remove_nickname);
}

inline static void __kick_only_one_channel_given(
	std::map<ChannelName, Channel>    &channels_by_name,
	std::map<NickName, Client *const> &clients_by_nickname,
	Client                            &sender,
	ChannelNameVector                  split_channels_names,
	NickNameVector                     split_nicknames)
{
	ChannelName const &chan_name = split_channels_names[0];

	if (channels_by_name.count(chan_name))
		return sender.append_formatted_reply_to_msg_out(ERR_NOSUCHCHANNEL, &chan_name);

	Channel &channel = channels_by_name.find(chan_name)->second;

	if (channel.get_modes().has_operator(sender) == false)
		return sender.append_formatted_reply_to_msg_out(ERR_CHANOPRIVSNEEDED, &chan_name);

	for (size_t i = 0; !split_nicknames[i].empty(); ++i)
	{
		if (clients_by_nickname.count(split_nicknames[i]) == 0)
			sender.append_formatted_reply_to_msg_out(ERR_USERNOTONCHANNEL, &split_nicknames[i]);

		Client &user_to_remove = *clients_by_nickname[split_nicknames[i]];

		if (channel.has_member(user_to_remove) == false)
			sender.append_formatted_reply_to_msg_out(ERR_USERNOTONCHANNEL, &split_nicknames[i], &chan_name);
		else
			__kick_client(channel, chan_name, NickName(split_nicknames[i]), user_to_remove);
	}
}

inline static void __kick_multiple_channel_given(
	std::map<ChannelName, Channel>    &channels_by_name,
	std::map<NickName, Client *const> &clients_by_nickname,
	Client                            &sender,
	ChannelNameVector                  split_channels_names,
	NickNameVector                     split_nicknames,
	size_t                             split_channels_names_size)
{
	for (size_t i = 0; i < split_channels_names_size; ++i)
	{
		if (channels_by_name.count(split_channels_names[i]) == 0)
			sender.append_formatted_reply_to_msg_out(ERR_NOSUCHCHANNEL, &split_channels_names[i]);
		else
		{
			ChannelName const &chan_name = split_channels_names[i];
			Channel           &channel = channels_by_name[chan_name];

			if (clients_by_nickname.count(split_nicknames[i]) == 0)
				sender.append_formatted_reply_to_msg_out(ERR_USERNOTONCHANNEL, &split_nicknames[i]);

			Client &user_to_remove = *clients_by_nickname.find(split_nicknames[i])->second;

			if (!channel.get_modes().has_operator(sender))
				sender.append_formatted_reply_to_msg_out(ERR_CHANOPRIVSNEEDED, &chan_name);
			else if (!channel.has_member(user_to_remove))
				sender.append_formatted_reply_to_msg_out(ERR_USERNOTONCHANNEL, &split_nicknames[i], &chan_name);
			else
				__kick_client(channel, chan_name, split_nicknames[i], user_to_remove);
		}
	}
}

void Server::_kick(Client &sender, std::vector<std::string> const &params)
{
	if (params.size() < 2)
		return sender.append_formatted_reply_to_msg_out(ERR_NEEDMOREPARAMS, "KICK");

	ChannelNameVector split_channels_names = split<ChannelNameVector>(params[0], ',');
	NickNameVector    split_nicknames = split<NickNameVector>(params[1], ',');

	size_t split_channels_names_size = split_channels_names.size();

	if (split_channels_names_size != 1 && split_nicknames.size() != split_channels_names_size)
		return sender.append_formatted_reply_to_msg_out(ERR_NEEDMOREPARAMS, "KICK");

	if (split_channels_names_size == 1)
		__kick_only_one_channel_given(
			this->_channels_by_name,
			this->_clients_by_nickname,
			sender,
			split_channels_names,
			split_nicknames);
	else
		__kick_multiple_channel_given(
			this->_channels_by_name,
			this->_clients_by_nickname,
			sender,
			split_channels_names,
			split_nicknames,
			split_channels_names_size);
}
