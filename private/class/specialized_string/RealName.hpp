/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RealName.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 22:46:16 by jodufour          #+#    #+#             */
/*   Updated: 2024/02/24 00:28:29 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>

class RealName : public std::string
{
public:
	// Constructors
	RealName(std::string const &realname = "");

	// Destructor
	~RealName(void);
};
