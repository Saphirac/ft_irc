/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/24 19:31:04 by mcourtoi          #+#    #+#             */
/*   Updated: 2023/11/28 18:13:02 by mcourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"
#include <cstdlib>

int main(int ac, char **av)
{
	if (ac != 2)
		return 1;
	create_server(atoi(av[1]));
	return 0;
}
