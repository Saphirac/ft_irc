/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replies.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 22:08:30 by jodufour          #+#    #+#             */
/*   Updated: 2024/02/05 22:00:16 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REPLIES_HPP
#define REPLIES_HPP

#include <map>
#include <string>
#include <utility>

#define RPL_WELCOME  001
#define RPL_YOURHOST 002
#define RPL_CREATED  003
#define RPL_MYINFO   004

#define RPL_UMODEIS         221
#define RPL_AWAY            301
#define RPL_ISON            303
#define RPL_UNAWAY          305
#define RPL_NOWAWAY         306
#define RPL_WHOISUSER       311
#define RPL_WHOISOPERATOR   313
#define RPL_ENDOFWHOIS      318
#define RPL_WHOISCHANNELS   319
#define RPL_LIST            322
#define RPL_LISTEND         323
#define RPL_CHANNELMODEIS   324
#define RPL_NOTOPIC         331
#define RPL_TOPIC           332
#define RPL_INVITING        341
#define RPL_INVITELIST      346
#define RPL_ENDOFINVITELIST 347
#define RPL_VERSION         351
#define RPL_NAMESREPLY      353
#define RPL_ENDOFNAMES      366
#define RPL_BANLIST         367
#define RPL_ENDOFBANLIST    368
#define RPL_INFO            371
#define RPL_MOTD            372
#define RPL_ENDOFINFO       374
#define RPL_MOTDSTART       375
#define RPL_ENDOFMOTD       376
#define RPL_YOUREOPER       381
#define RPL_TIME            391

#define ERR_NOSUCHNICK        401
#define ERR_NOSUCHCHANNEL     403
#define ERR_CANNOTSENDTOCHAN  404
#define ERR_TOOMANYCHANNELS   405
#define ERR_NOORIGIN          409
#define ERR_NORECIPIENT       411
#define ERR_NOTEXTTOSEND      412
#define ERR_NOMOTD            422
#define ERR_NONICKNAMEGIVEN   431
#define ERR_ERRONEUSNICKNAME  432
#define ERR_NICKNAMEINUSE     433
#define ERR_USERNOTONCHANNEL  441
#define ERR_NOTONCHANNEL      442
#define ERR_USERONCHANNEL     443
#define ERR_NEEDMOREPARAMS    461
#define ERR_ALREADYREGISTERED 462
#define ERR_PASSWDMISMATCH    464
#define ERR_KEYSET            467
#define ERR_CHANNELISFULL     471
#define ERR_UNKNOWNMODE       472
#define ERR_INVITEONLYCHAN    473
#define ERR_BANNEDFROMCHAN    474
#define ERR_BADCHANNELKEY     475
#define ERR_NOCHANMODES       477
#define ERR_NOPRIVILEGES      481
#define ERR_CHANOPRIVSNEEDED  482
#define ERR_NOOPERHOST        491
#define ERR_UMODEUNKNOWNFLAG  501
#define ERR_USERSDONTMATCH    502

static std::pair<int, char const *> const RAW_REPLIES[] = {
	std::make_pair(RPL_WELCOME, ":Welcome to the Internet Relay Network %s!%s@%s"),
	std::make_pair(RPL_YOURHOST, ":Your host is %s, running version %s"),
	std::make_pair(RPL_CREATED, ":This server was created %s"),
	std::make_pair(RPL_MYINFO, "%s %s %s %s"),
	std::make_pair(RPL_UMODEIS, "%s :+%s"),
	std::make_pair(RPL_AWAY, "%s :%s"),
	std::make_pair(RPL_ISON, ":%s"),
	std::make_pair(RPL_UNAWAY, ":You are no longer marked as being away"),
	std::make_pair(RPL_NOWAWAY, ":You have been marked as being away"),
	std::make_pair(RPL_WHOISUSER, "%s %s %s * :%s"),
	std::make_pair(RPL_WHOISOPERATOR, "%s :is an IRC operator"),
	std::make_pair(RPL_ENDOFWHOIS, "%s :End of WHOIS list"),
	std::make_pair(RPL_WHOISCHANNELS, "%s :%s"),
	std::make_pair(RPL_LIST, "%s %hhu :%s"),
	std::make_pair(RPL_LISTEND, ":End of LIST"),
	std::make_pair(RPL_CHANNELMODEIS, "%s %s %s"),
	std::make_pair(RPL_NOTOPIC, "%s :No topic is set"),
	std::make_pair(RPL_TOPIC, "%s :%s"),
	std::make_pair(RPL_INVITING, "%s %s"),
	std::make_pair(RPL_INVITELIST, "%s %s"),
	std::make_pair(RPL_ENDOFINVITELIST, "%s :End of channel invite list"),
	std::make_pair(RPL_VERSION, "%s %s :%s"),
	std::make_pair(RPL_NAMESREPLY, "%s :%s"),
	std::make_pair(RPL_ENDOFNAMES, "%s :End of NAMES list"),
	std::make_pair(RPL_BANLIST, "%s %s"),
	std::make_pair(RPL_ENDOFBANLIST, "%s :End of channel ban list"),
	std::make_pair(RPL_INFO, ":%s"),
	std::make_pair(RPL_MOTD, ":- %s"),
	std::make_pair(RPL_ENDOFINFO, ":End of INFO list"),
	std::make_pair(RPL_MOTDSTART, ":- %s Message of the day - "),
	std::make_pair(RPL_ENDOFMOTD, ":End of MOTD command"),
	std::make_pair(RPL_YOUREOPER, ":You are now an IRC operator"),
	std::make_pair(RPL_TIME, "%s :%s"),
	std::make_pair(ERR_NOSUCHNICK, "%s :No such nick/channel"),
	std::make_pair(ERR_NOSUCHCHANNEL, "%s :No such channel"),
	std::make_pair(ERR_CANNOTSENDTOCHAN, "%s :Cannot send to channel"),
	std::make_pair(ERR_TOOMANYCHANNELS, "%s :You have joined too many channels"),
	std::make_pair(ERR_NOORIGIN, ":No origin specified"),
	std::make_pair(ERR_NORECIPIENT, ":No recipient given (%s)"),
	std::make_pair(ERR_NOTEXTTOSEND, ":No text to send"),
	std::make_pair(ERR_NOMOTD, ":MOTD File is missing"),
	std::make_pair(ERR_NONICKNAMEGIVEN, ":No nickname given"),
	std::make_pair(ERR_ERRONEUSNICKNAME, "%s :Erroneous nickname"),
	std::make_pair(ERR_NICKNAMEINUSE, "%s :Nickname is already in use"),
	std::make_pair(ERR_USERNOTONCHANNEL, "%s %s :They aren't on that channel"),
	std::make_pair(ERR_NOTONCHANNEL, "%s :You're not on that channel"),
	std::make_pair(ERR_USERONCHANNEL, "%s %s :is already on channel"),
	std::make_pair(ERR_NEEDMOREPARAMS, "%s :Not enough parameters"),
	std::make_pair(ERR_ALREADYREGISTERED, ":Unauthorized command (already registered)"),
	std::make_pair(ERR_PASSWDMISMATCH, ":Password incorrect"),
	std::make_pair(ERR_KEYSET, "%s :Channel key already set"),
	std::make_pair(ERR_CHANNELISFULL, "%s :Cannot join channel (+l)"),
	std::make_pair(ERR_UNKNOWNMODE, "{char} :is unknown mode char to me for %s"),
	std::make_pair(ERR_INVITEONLYCHAN, "%s :Cannot join channel (+i)"),
	std::make_pair(ERR_BANNEDFROMCHAN, "%s :Cannot join channel (+b)"),
	std::make_pair(ERR_BADCHANNELKEY, "%s :Cannot join channel (+k)"),
	std::make_pair(ERR_NOCHANMODES, "%s :Channel doesn't support modes"),
	std::make_pair(ERR_NOPRIVILEGES, ":Permission Denied- You're not an IRC operator"),
	std::make_pair(ERR_CHANOPRIVSNEEDED, "%s :You're not channel operator"),
	std::make_pair(ERR_NOOPERHOST, ":No O-lines for your host"),
	std::make_pair(ERR_UMODEUNKNOWNFLAG, ":Unknown MODE flag"),
	std::make_pair(ERR_USERSDONTMATCH, ":Cannot change mode for other users"),
};
static size_t const RAW_REPLIES_LEN = sizeof(RAW_REPLIES) / sizeof(*RAW_REPLIES);

static std::map<int, char const *> const REPLIES(RAW_REPLIES, RAW_REPLIES + RAW_REPLIES_LEN);

typedef std::map<int, char const *>::const_iterator ReplyIterator;

#endif