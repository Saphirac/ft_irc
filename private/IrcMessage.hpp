/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcMessage.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 14:06:14 by mcourtoi          #+#    #+#             */
/*   Updated: 2024/02/17 23:04:16 by mcourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRCMESSAGE_HPP
#define IRCMESSAGE_HPP

#include "ft_irc.hpp"

class IrcMessage
{
private:

	std::string              _prefix;
	std::string              _command;
	std::vector<std::string> _params;

public:

	void display() const;
	std::string	get_prefix() const;
	std::string	get_command() const;
	std::vector<std::string>	*get_params();

	void set_prefix(const std::string &prefix);
	void set_command(const std::string &command);
	void set_params(const std::vector<std::string> &params);
};

IrcMessage parse_irc_message(const std::string &message);

#endif
