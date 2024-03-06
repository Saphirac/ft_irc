/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   methods.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/04 22:51:47 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/05 15:40:00 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "abnf_components.hpp"
#include "class/specialized_string/Key.hpp"

/**
 * @brief Checks whether the key is valid.
 *
 * @return `true` if the key is valid, `false` otherwise.
 */
bool Key::is_valid(void) const
{
	return lfcrspcl.find(this->at(0)) == std::string::npos && this->find_first_of(lfcrsp, 1) == std::string::npos;
}
