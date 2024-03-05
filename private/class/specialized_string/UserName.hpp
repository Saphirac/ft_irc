/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UserName.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 22:44:12 by jodufour          #+#    #+#             */
/*   Updated: 2024/02/24 00:28:51 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>

class UserName : public std::string
{
public:
	// Constructors
	UserName(std::string const &username = "");

	// Destructor
	~UserName(void);

	// Methods
	bool is_valid(void) const;
};
