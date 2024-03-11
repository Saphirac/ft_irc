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
	std::string const              &get_prefix(void) const;
	std::string const              &get_command(void) const;
	std::vector<std::string> const &get_parameters(void) const;

private:
	// Fields
	std::string              _prefix;
	std::string              _command;
	std::vector<std::string> _parameters;
};
