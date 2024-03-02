/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Hostname.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 22:45:24 by jodufour          #+#    #+#             */
/*   Updated: 2024/02/17 22:45:32 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>

class Hostname : public std::string
{
public:
	// Constructors
	Hostname(std::string const &hostname = "");
	Hostname(Hostname const &src);

	// Destructor
	~Hostname(void);

	// Methods
	bool is_valid(void) const;
};
