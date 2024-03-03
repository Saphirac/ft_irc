/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 14:06:05 by mcourtoi          #+#    #+#             */
/*   Updated: 2024/02/26 16:57:57 by mcourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcMessage.hpp"

void IrcMessage::setPrefix(const std::string& prefix) {
    _prefix = prefix;
}

void IrcMessage::setCommand(const std::string& command) {
    _command = command;
}

void IrcMessage::setParams(const std::vector<std::string>& params) {
    _params = params;
}

void IrcMessage::addParam(const std::string& param) {
    _params.push_back(param);
}

std::string const &IrcMessage::get_prefix() const {
    return _prefix;
}

std::string const &IrcMessage::get_command() const {
    return _command;
}

std::vector<std::string> &IrcMessage::get_params() {
    return _params;
}

std::ostream& operator<<(std::ostream& os, const IrcMessage& msg) {
    os << "Prefix: " << msg._prefix << std::endl
       << "Command: " << msg._command << std::endl
       << "Params: ";
    for (size_t i = 0; i < msg._params.size(); ++i) {
        os << "[" << msg._params[i] << "]";
        if (i < msg._params.size() - 1) os << ", ";
    }
    os << std::endl;
    return os;
}

IrcMessage::~IrcMessage() {}

IrcMessage::IrcMessage(const IrcMessage& other) 
    : _prefix(other._prefix), 
      _command(other._command), 
      _params(other._params) 
{}

IrcMessage& IrcMessage::operator=(const IrcMessage& other) 
{
    if (this != &other) 
    {
        _prefix = other._prefix;
        _command = other._command;
        _params = other._params;
    }
    return *this;
}
