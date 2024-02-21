/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcMessage.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 14:06:05 by mcourtoi          #+#    #+#             */
/*   Updated: 2024/02/18 00:36:21 by mcourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcMessage.hpp"

// Getters //

std::string IrcMessage::get_prefix() const { return this->_prefix; }
std::string IrcMessage::get_command() const { return this->_command; }
std::vector<std::string> *IrcMessage::get_params() { return &this->_params; }

// Setters //

void	IrcMessage::set_prefix(const std::string &prefix) { this->_prefix = prefix; }
void	IrcMessage::set_command(const std::string &command) { this->_command = command; }
void	IrcMessage::set_params(const std::vector<std::string> &params) { this->_params = params; }

void IrcMessage::display() const 
{
    std::cout << "Prefix: [" << _prefix << "]\n";
    std::cout << "Command: [" << _command << "]\n";
    std::cout << "Params: " << "\n";
    for (size_t i = 0; i < _params.size(); ++i) {
        std::cout << " [" << _params[i] << "]\n";
    }
}
