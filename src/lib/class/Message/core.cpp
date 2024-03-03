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

#include "Message.hpp"

/**
 * Constructor for the Message class.
 * Parses a single message from the given string.
 *
 * @param msg A constant reference to a std::string containing the message to be parsed.
 */
Message::Message(std::string const &msg) { this->parseSingleMessage(msg); }

/**
 * Destructor for the Message class.
 */
Message::~Message() {}

/**
 * Parses a single message from the given string.
 *
 * @param msg A constant reference to a std::string containing the message to be parsed.
 */
std::string const &Message::get_prefix() const { return _prefix; }

/**
 * Returns the command of the message.
 *
 * @return A constant reference to a std::string containing the command of the message.
 */
std::string const &Message::get_command() const { return _command; }

/**
 * Returns the parameters of the message.
 *
 * @return A reference to a vector of std::string containing the parameters of the message.
 */
std::vector<std::string> &Message::get_params() { return _params; }

/**
 * Sets the prefix of the message.
 *
 * @param prefix A constant reference to a std::string containing the prefix to be set.
 */
void Message::set_prefix(const std::string &prefix) { _prefix = prefix; }

/**
 * Sets the command of the message.
 *
 * @param command A constant reference to a std::string containing the command to be set.
 */
void Message::set_command(const std::string &command) { _command = command; }

/**
 * Sets the parameters of the message.
 *
 * @param params A constant reference to a vector of std::string containing the parameters to be set.
 */
void Message::set_params(const std::vector<std::string> &params) { _params = params; }

/**
 * Adds a single parameter to the message's parameters.
 *
 * @param param A constant reference to a std::string containing the parameter to be added.
 */
void Message::add_param(const std::string &param) { _params.push_back(param); }
