/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcMessage.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 14:06:14 by mcourtoi          #+#    #+#             */
/*   Updated: 2024/02/17 18:29:50 by mcourtoi         ###   ########.fr       */
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
	std::vector<std::string>	get_params() const;
};

IrcMessage parse_irc_message(const std::string &message);

#endif
