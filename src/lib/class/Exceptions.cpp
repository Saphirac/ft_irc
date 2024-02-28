/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Exceptions.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodufour <jodufour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 11:13:05 by jodufour          #+#    #+#             */
/*   Updated: 2024/02/28 17:29:21 by jodufour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/Exceptions.hpp"

// ***************************************************************************************************************** //
//                                                      Methods                                                      //
// ***************************************************************************************************************** //
char const *NotAFlag::what(void) const throw() { return "The given mode is not a flag."; }

char const *ProblemWithFormatReply::what(void) const throw() { return "Problem with format_reply()."; }

char const *ProblemWithSend::what(void) const throw() { return "Problem with send()."; }
