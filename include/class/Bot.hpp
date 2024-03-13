/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 18:56:18 by mcourtoi          #+#    #+#             */
/*   Updated: 2024/03/13 19:04:54 by mcourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>

class Bot
{
public:

	Bot(int port, std::string const &password);

	~Bot();
	
	void run();
	

private:

	int _sockfd;

	void _send_connexion_message()


};

void handle_sigint(int sig);
