/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/04 22:31:15 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/04 22:32:38 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/specialized_string/ChannelName.hpp"

// Constructors //

/**
 * @param channel_name The channel name to initialize the instance with.
 */
ChannelName::ChannelName(std::string const &channel_name) : std::string(channel_name) {}

// Destructor //

ChannelName::~ChannelName(void) {}
