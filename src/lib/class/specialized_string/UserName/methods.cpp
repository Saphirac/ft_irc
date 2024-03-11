/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   methods.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gle-mini <gle-mini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/04 22:42:26 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/11 16:21:24 by gle-mini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/specialized_string/UserName.hpp"

/**
 * @brief Check whether the username is valid.
 *
 * @return `true` if the username is valid, `false` otherwise.
 */
bool UserName::is_valid(void) const
{
	return !this->empty() && this->find('@') == std::string::npos;
}
