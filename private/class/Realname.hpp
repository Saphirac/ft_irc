/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Realname.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 22:46:16 by jodufour          #+#    #+#             */
/*   Updated: 2024/02/17 22:46:23 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>

class Realname : public std::string
{
public:
	// Constructors
	Realname(std::string const &realname = "");
	Realname(Realname const &src);

	// Destructor
	~Realname(void);

	// Methods
	bool is_valid(void) const;
};
