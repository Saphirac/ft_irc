/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HostName.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 22:45:24 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/02 18:00:24 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>

class HostName : public std::string
{
public:
	// Constructors
	HostName(std::string const &hostname = "");

	// Destructor
	~HostName(void);

	// Methods
	bool is_valid(void) const;
};
