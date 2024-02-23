/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Username.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 22:44:12 by jodufour          #+#    #+#             */
/*   Updated: 2024/02/17 22:44:47 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>

class Username : public std::string
{
public:
	// Constructors
	Username(std::string const &username = "");
	Username(Username const &src);

	// Destructor
	~Username(void);

	// Methods
	bool is_valid(void) const;
};
