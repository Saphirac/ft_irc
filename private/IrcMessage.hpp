/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcMessage.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 14:06:14 by mcourtoi          #+#    #+#             */
/*   Updated: 2024/02/28 14:51:23 by mcourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRCMESSAGE_HPP
#define IRCMESSAGE_HPP

#include <string>
#include <vector>

class IrcMessage
{
private:
	std::string              _prefix;
	std::string              _command;
	std::vector<std::string> _params;
	std::string              _end;

	bool _is_complete;

public:
	IrcMessage();
	~IrcMessage();

	// Getters //
	std::string const        &get_prefix() const;
	std::string const        &get_command() const;
	std::vector<std::string> &get_params();
	std::string const        &get_end() const;

	bool is_complete() const;

	void       display() const;
	IrcMessage parse_irc_message(std::string const &msg);
};

#endif
