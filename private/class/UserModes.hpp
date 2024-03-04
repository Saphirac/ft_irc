/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UserModes.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/18 23:03:36 by jodufour          #+#    #+#             */
/*   Updated: 2024/02/29 17:20:40 by mcourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "UserMode.hpp"
#include <string>

class UserModes
{
private:
	// Types
	typedef int _inner_type;

	// Fields
	_inner_type _inner;

public:
	// Constructors
	UserModes(_inner_type const modes = 0);
	UserModes(UserModes const &src);

	// Destructor
	~UserModes(void);

	// Methods
	void clear(UserMode const mode);
	void set(UserMode const mode);

	bool is_set(UserMode const mode) const;

	std::string to_string(void) const;
};
