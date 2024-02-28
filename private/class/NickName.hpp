/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NickName.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 03:36:23 by jodufour          #+#    #+#             */
/*   Updated: 2024/02/24 00:28:26 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>

class NickName : public std::string
{
public:
	// Constructors
	NickName(std::string const &nickname = "");

	// Destructor
	~NickName(void);

	// Methods
	bool is_valid(void) const;
};
