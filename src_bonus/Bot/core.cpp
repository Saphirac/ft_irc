/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 18:57:11 by mcourtoi          #+#    #+#             */
/*   Updated: 2024/03/14 15:20:25 by mcourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/Bot.hpp"
#include "class/exception/ProblemWithConnect.hpp"
#include "class/exception/ProblemWithSocket.hpp"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <signal.h>
#include <string>
#include <sys/socket.h>


extern volatile bool bot_interrupted;

inline static void handle_sigint(int signal_number __attribute__((unused))) { bot_interrupted = true; }

Bot::Bot(int const port, std::string const &password) : _socket(socket(AF_INET, SOCK_STREAM, 0)), _password(password)
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
	if (connect(this->_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
		throw ProblemWithConnect();

	signal(SIGINT, handle_sigint);
}

Bot::~Bot() { this->_disconnect(); }
