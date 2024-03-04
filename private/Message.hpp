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
	Message(std::string const &msg);
	
	// Destructor
	~Message(void);

	// Accessors
	std::string const        &get_prefix() const;
	std::string const        &get_command() const;
	std::vector<std::string> &get_params();

	// Setters
	void set_prefix(std::string const &prefix);
	void set_command(std::string const &command);
	void set_params(std::vector<std::string> const &params);

	// Methods
	void add_param(std::string const &param);

	void display() const;

private:
	std::string              _prefix;
	std::string              _command;
	std::vector<std::string> _params;
};
