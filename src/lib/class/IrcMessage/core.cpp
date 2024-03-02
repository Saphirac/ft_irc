/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 14:06:05 by mcourtoi          #+#    #+#             */
/*   Updated: 2024/03/02 00:48:26 by mcourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcMessage.hpp"

// Getters //

std::string const              &IrcMessage::get_prefix() const { return this->_prefix; }
std::string const              &IrcMessage::get_command() const { return this->_command; }
std::vector<std::string> const &IrcMessage::get_params() { return this->_params; }
std::string const              &IrcMessage::get_end() const { return this->_end; }
