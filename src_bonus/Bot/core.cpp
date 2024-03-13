/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 18:57:11 by mcourtoi          #+#    #+#             */
/*   Updated: 2024/03/13 19:12:18 by mcourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/Bot.hpp"
#include <cstdlib>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include "class/Message.hpp"
#include <signal.h>
#include <iostream>
#include "class/exception/ProblemWithSocket.hpp"

Bot::Bot(int const port, std::string const &password) :
_socket(socket(AF_INET, SOCK_STREAM, 0)),
_password(password)
{
	// Création du socket
	if (this->_socket < 0)
		throw ProblemWithSocket();

	// Préparation de l'adresse du serveur
	sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	// Connexion au serveur
	if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
		throw ProblemWithConnect();

	struct sigaction sa;
	sa.sa_handler = handle_sigint; // Définir le gestionnaire de signal
	sigemptyset(&sa.sa_mask);	   // Bloquer aucun signal supplémentaire
	sa.sa_flags = 0;			   // Flags à 0

	if (sigaction(SIGINT, &sa, NULL) == -1)
		throw ProblemWithSigaction();
}

Bot::~Bot()
{
	close(this->_socket);
}
