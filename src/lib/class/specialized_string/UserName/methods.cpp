/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   methods.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/04 22:42:26 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/04 22:42:32 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/specialized_string/UserName.hpp"

/**
 * @brief Check whether the username is valid.
 *
 * @return `true` if the username is valid, `false` otherwise.
 */
bool UserName::is_valid(void) const { return this->find('@') == std::string::npos; }
