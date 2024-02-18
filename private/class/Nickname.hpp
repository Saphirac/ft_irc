/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nickname.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 03:36:23 by jodufour          #+#    #+#             */
/*   Updated: 2024/02/19 00:11:12 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>

class Nickname : public std::string
{
public:
	// Constructors
	Nickname(std::string const &nickname = "");
	Nickname(Nickname const &src);

	// Destructor
	~Nickname(void);

	// Methods
	bool is_valid(void) const;
};
