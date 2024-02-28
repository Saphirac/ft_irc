/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Topic.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/24 00:26:18 by jodufour          #+#    #+#             */
/*   Updated: 2024/02/24 00:34:25 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>

class Topic : public std::string
{
public:
	// Constructors
	Topic(std::string const &topic = "");

	// Destructor
	~Topic(void);

	// Methods
	bool is_valid(void) const;
};
