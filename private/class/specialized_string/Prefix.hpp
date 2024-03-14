/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Prefix.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/10 03:04:01 by gle-mini          #+#    #+#             */
/*   Updated: 2024/03/13 17:26:18 by mcourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>

class Prefix : public std::string
{
public:
	// Constructors
	Prefix(std::string const &prefix = "");

	// Destructor
	~Prefix(void);

	// Methods
	bool is_valid(void) const;

	// Getters
	std::string const who_is_sender(void) const;
};
