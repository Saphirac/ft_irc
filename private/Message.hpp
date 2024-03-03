/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 14:06:14 by mcourtoi          #+#    #+#             */
/*   Updated: 2024/02/26 16:58:04 by mcourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
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

private:
	std::string              _prefix;
	std::string              _command;
	std::vector<std::string> _params;
	void                     parseSingleMessage(const std::string &msg);
};
