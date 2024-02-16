/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcMessage.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 14:06:05 by mcourtoi          #+#    #+#             */
/*   Updated: 2024/02/17 18:31:59 by mcourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcMessage.hpp"

// Getters //

std::string IrcMessage::get_prefix() const { return _prefix; }
std::string IrcMessage::get_command() const { return _command; }
std::vector<std::string> IrcMessage::get_params() const { return _params; }

void IrcMessage::display() const 
{
    std::cout << "Prefix: " << _prefix << std::endl;
    std::cout << "Command: " << _command << std::endl;
    std::cout << "Params:" << std::endl;
    for (size_t i = 0; i < _params.size(); ++i) {
        std::cout << "  " << _params[i] << std::endl;
    }
}
