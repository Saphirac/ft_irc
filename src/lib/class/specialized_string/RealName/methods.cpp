/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   methods.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 00:45:56 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/11 01:40:26 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "abnf_components.hpp"
#include "class/specialized_string/RealName.hpp"

/**
 * @brief Checks whether the real name is valid.
 *
 * @return `true` if the real name is valid, `false` otherwise.
 */
bool RealName::is_valid(void) const { return !this->empty() }
