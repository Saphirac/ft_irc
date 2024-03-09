/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   methods.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/04 22:50:00 by jodufour          #+#    #+#             */
/*   Updated: 2024/03/04 22:50:51 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/specialized_string/Topic.hpp"

/**
 * @brief Checks whether the topic is valid.
 *
 * @return `true` if the topic is valid, `false` otherwise.
 */
bool Topic::is_valid(void) const { return this->find_first_of("\r\n") == this->npos; }
