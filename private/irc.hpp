/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gle-mini <gle-mini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/24 19:31:52 by mcourtoi          #+#    #+#             */
/*   Updated: 2024/01/15 15:59:06 by mcourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRC_HPP
# define IRC_HPP

# ifndef DEBUG
#  define DEBUG 0
# endif

# include <iostream>
# include <vector>
# include "regex.hpp"

int hello(void);

// Utils //
std::string	trim(std::string const &str);

#endif
