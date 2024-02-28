/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_map_cmd.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 22:05:21 by mcourtoi          #+#    #+#             */
/*   Updated: 2024/02/28 17:33:26 by mcourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/Server.hpp"

/**
 * @brief init a map with the pointer to the function of each command with the cmd name as key
 * 
 */
void Server::init_map_cmd(void)
{
	this->_map_of_cmds["CAP"] = &Server::cap;
	this->_map_of_cmds["USER"] = &Server::user;
	this->_map_of_cmds["NICK"] = &Server::nick;
	this->_map_of_cmds["JOIN"] = &Server::join;
	this->_map_of_cmds["PASS"] = &Server::pass;
}

/*static std::pair<std::string, Server::cmd> const raw_cmd [] = {
    {"CAP", Server::cap},
    {"USER", Server::user},
};

static size_t const raw_cmd_len = sizeof(raw_cmd) / sizeof(*raw_cmd);

static std::map<std::string, Server::cmd> const map_of_cmds(raw_cmd, raw_cmd + raw_cmd_len);
*/
