/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 02:53:52 by mcourtoi          #+#    #+#             */
/*   Updated: 2024/02/20 21:21:44 by mcourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/Client.hpp"

/**
 * @brief Appends a given message to the messages of the Client instance.
 *
 * @param message The message to append.
 */
void Client::append_message(std::string const &message) { this->_messages += message + "\r\n"; }

/**
 * @brief Clears the messages of the Client instance.
 */
void Client::clear_messages(void) { this->_messages.clear(); }

/**
 * @brief Sets a given mode for the client.
 *
 * @param mode The mode to set.
 */
void Client::set_mode(UserMode const mode) { this->_modes |= 1 << mode; }

/**
 * @brief Clears a given mode for the client.
 *
 * @param mode The mode to clear.
 */
void Client::clear_mode(UserMode const mode) { this->_modes &= ~(1 << mode); }

/**
 * @brief Check whether the client has a given mode set.
 *
 * @param mode The mode to check.
 *
 * @return true if the client has the given mode set, false otherwise.
 */
bool Client::has_mode(UserMode const mode) const { return this->_modes & 1 << mode; }

/**
 * @brief Sends a given message on the socket of the Client instance.
 *
 * @param message The message to send.
 *
 * @return The number of bytes sent, or -1 if an error occurred.
 */
ssize_t Client::send_message(std::string const &message) const
{
	return send(this->_socket, message.c_str(), message.size(), 0);
}

/**
 * @brief Sends the messages that are currently stored in the Client instance on its socket.
 *
 * @return The number of bytes sent, or -1 if an error occurred.
 */
ssize_t Client::send_messages(void) const
{
	return send(this->_socket, this->_messages.c_str(), this->_messages.size(), 0);
}

/**
 * @brief Generates the user mask of the client.
 *
 * @return The user mask of the client.
 */
std::string Client::user_mask(void) const { return this->_nickname + "!" + this->_username + "@" + this->_hostname; }

std::clock_t Client::check_time_since_last_msg(void) const { return std::clock() - this->_time_last_msg; }
