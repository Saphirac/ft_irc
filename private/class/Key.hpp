/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Key.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/24 01:08:06 by jodufour          #+#    #+#             */
/*   Updated: 2024/02/24 01:09:06 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>

class Key : public std::string
{
public:
	// Constructors
	Key(std::string const &key = "");

	// Destructor
	~Key(void);

	// Methods
	bool is_valid(void) const;
};
