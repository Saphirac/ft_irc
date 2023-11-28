/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/24 19:31:52 by mcourtoi          #+#    #+#             */
/*   Updated: 2023/11/28 18:27:29 by mcourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRC
# define IRC

# ifndef DEBUG
#  define DEBUG 0
# endif

# include <iostream>

// Utils //
std::string	trim(std::string const &str);
void		create_server(int chosen_addr);

#endif
