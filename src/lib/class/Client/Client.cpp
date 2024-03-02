/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 02:53:52 by mcourtoi          #+#    #+#             */
/*   Updated: 2024/03/02 03:14:47 by mcourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/Client.hpp"
#include "class/Exceptions.hpp"

#define TERMINATING_SEQ "\r\n"
#define MAX_MSG_SIZE    512

/**
 * @brief Appends a string to the input buffer of the Client instance.
 *
 * @param s The string to append.
 */
void Client::append_to_msg_in(std::string const &s) { this->_msg_in += s; }

/**
 * @brief Appends a message to the output buffer of the Client instance. The message is suffixed with a CRLF sequence.
 *
 * @param msg The message to append.
 */
void Client::append_to_msg_out(std::string const &msg) { this->_msg_out += msg + "\r\n"; }

/**
 * @brief Clears the output buffer of the Client instance.
 */
void Client::clear_msg_out(void) { this->_msg_out.clear(); }

/**
 * @brief Sets a given mode for the client.
 *
 * @param mode The mode to set.
 */
void Client::set_mode(UserMode const mode) { this->_modes.set(mode); }

/**
 * @brief Clears a given mode for the client.
 *
 * @param mode The mode to clear.
 */
void Client::clear_mode(UserMode const mode) { this->_modes.clear(mode); }

/**
 * @brief Check whether the client has a given mode set.
 *
 * @param mode The mode to check.
 *
 * @return true if the client has the given mode set, false otherwise.
 */
bool Client::has_mode(UserMode const mode) const { return this->_modes.is_set(mode); }

/**
 * @brief send the message stocked in the _msg_out buffer to the client.
 *
 * @throw ProblemWithSend() if the send() function fails.
 */
void Client::send_msg_out(void)
{
	if (send(this->_socket, this->_msg_out.c_str(), this->_msg_out.size(), 0) == -1)
		throw ProblemWithSend();
	this->clear_msg_out();
}

/**
 * @brief Generates the user mask of the client.
 *
 * @return The user mask of the client.
 */
std::string Client::user_mask(void) const { return this->_nickname + "!" + this->_username + "@" + this->_hostname; }

/**
 * @brief Compare the time since the last message was sent by the client to the current time.
 *
 * @return the difference between the current time and the time of the last message.
 */
std::clock_t Client::check_time_since_last_msg(void) const { return std::clock() - this->_time_last_msg; }

/**
 * @brief Get the first message in the input buffer of the Client instance.
 * A message is <= 512 characters and is suffixed with a CRLF sequence.
 * Then it extracts and return this message and erase it from the reste of the buffer.
 *
 * @return the msg found or an empty string if no message is found / the msg is too long
 */
std::string const Client::get_next_msg()
{
	size_t const pos = this->_msg_in.find(TERMINATING_SEQ);

	if (pos == std::string::npos)
		return std::string();

	std::string const msg = this->_msg_in.substr(0, pos);

	this->_msg_in.erase(0, pos + sizeof(TERMINATING_SEQ));
	if (pos > MAX_MSG_SIZE)
		return std::string();
	return msg;
}

std::string const Client::generate_prefix(std::string const &server_name) const
{
	std::string prefix = server_name + '/' + this->_nickname + '!' + this->_username + '@' + this->_hostname;
	return prefix;
}
