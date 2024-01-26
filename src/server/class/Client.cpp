/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 02:53:52 by mcourtoi          #+#    #+#             */
/*   Updated: 2024/01/26 04:37:16 by mcourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

Client::Client(int const socket, int const name, int const nickname) :
_socket(socket),
_name(name),
_nickname(nickname),
{
	if (DEBUG)
		std::cout << "Client constructor called\n";
}

Client::~Client()
{
	if (DEBUG)
		std::cout << "Client destructor called\n";
}

// Getters //

int	Client::getSocket(void) const
{
	if (DEBUG)
		std::cout << "getSocket() member function of client called\n";
	return this->_socket;
}

std::string	Client::getName(void) const
{
	if (DEBUG)
		std::cout << "getName() member function of client called\n";
	return this->_name;
}

std::string	Client::getNickname(void) const
{
	if (DEBUG)
		std::cout << "getNickname() member function of client called\n";
	return this->_nickname;
}

struct epoll_event	*Client::getEvent(void) const
{
	if (DEBUG)
		std::cout << "getEvent() member function of client called\n";
	return this->_client_event;
}

bool	Client::getIsComplete() const
{
	if (DEBUG)
		std::cout << "getIsComplete() member function of client called\n";
	return this->_is_complete;
}

std::string	Client::getInput() const
{
	if (DEBUG)
		std::cout << "getInput() member function of client called\n";
	return this->_input;
}

bool	Client::getIsInputComplete() const
{
	if (DEBUG)
		std::cout << "getIsInputComplete() member function of client called\n";
	return this->_is_input_complete;
}

Command	*Client::getCommand() const
{
	if (DEBUG)
		std::cout << "getCommand() member function of client called\n";
	return this->_command;
}

// Setters //

void	Client::setSocket(int const socket)
{
	if (DEBUG)
		std::cout << "setSocket() member function of client called\n";
	this->_socket = socket;
}

void	Client::setName(std::string const name)
{
	if (DEBUG)
		std::cout << "setName() member function of client called\n";
	this->_name = name;
}

void	Client::setNickname(std::string const nickname)
{
	if (DEBUG)
		std::cout << "setNickname() member function of client called\n";
	this->_nickname = nickname;
}

void	Client::setEvent()
{
	if (DEBUG)
		std::cout << "setEvent() member function of client called\n";
	this->_client_event = new struct epoll_event;
	this->_client_event->events = EPOLLIN;
	this->_client_event->data.fd = this->_socket;
}

void	Client::setIsComplete(bool const yesno)
{
	if (DEBUG)
		std::cout << "setIsComplete() member function of client called\n";
	this->_is_complete = yesno;
}

void	Client::setInput(std::string const input)
{
	if (DEBUG)
		std::cout << "setInput() member function of client called\n";
	this->_input = input;
}

void	Client::setIsInputComplete(bool const yesno)
{
	if (DEBUG)
		std::cout << "setIsInputComplete() member function of client called\n";
	this->_is_input_complete = yesno;
}

void	Client::setCommand(Command *command)
{
	if (DEBUG)
		std::cout << "setCommand() member function of client called\n";
	this->_command = command;
}

// Methods //


