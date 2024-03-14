/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Password.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gle-mini <gle-mini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 03:36:23 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/11 15:26:20 by gle-mini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>

class Password : public std::string
{
public:
	// Constructors
	Password(std::string const &password = "");

	// Destructor
	~Password(void);

	// Methods
	bool is_valid(void) const;
};
