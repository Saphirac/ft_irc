/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 14:06:14 by mcourtoi          #+#    #+#             */
/*   Updated: 2024/02/29 18:10:13 by mcourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "class/Server.hpp"
#include "class/specialized_string/Prefix.hpp"
#include <string>
#include <vector>

class Message
{
public:
	// Constructors
	Message(std::string const &raw_msg);

	// Destructor
	~Message(void);

	// Accessors
	Prefix const                         &get_prefix(void) const;
	std::string const                    &get_command(void) const;
	Server::CommandParameterVector const &get_parameters(void) const;

private:
	// Fields
	Prefix                         _prefix;
	std::string                    _command;
	Server::CommandParameterVector _parameters;
};
