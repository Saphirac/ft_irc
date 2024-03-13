/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Prefix.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/10 03:04:01 by gle-mini          #+#    #+#             */
/*   Updated: 2024/03/13 15:32:45 by mcourtoi         ###   ########.fr       */
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
	bool is_valid(void);

	// Getters
	std::string const &get_sender(void) const;

private :

	std::string _sender;
};
