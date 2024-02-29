/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_irc.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/24 19:31:52 by mcourtoi          #+#    #+#             */
/*   Updated: 2024/02/29 17:25:50 by mcourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# ifndef DEBUG
#  define DEBUG 0
# endif

# include <string>

// Utils //
std::string	trim(std::string const &str);

std::string format_reply(int const reply_number...);
