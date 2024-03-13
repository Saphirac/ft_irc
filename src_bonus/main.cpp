/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 13:13:11 by mcourtoi          #+#    #+#             */
/*   Updated: 2024/03/13 19:09:01 by mcourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#define RPL_INVITING "341"

bool run_bool = true;

void handle_sigint(int sig) 
{
	(void)sig;
	run_bool = false;
}

std::string remove_crlf(const std::string& str) 
{
	std::string result = str;

	// Vérifie si la chaîne se termine par CRLF et la réduit en conséquence
	if (result.length() >= 2 && result[result.length() - 2] == '\r' && result[result.length() - 1] == '\n') 
	{
		result.erase(result.length() - 2);
	}
	return result;
}

int main(int const ac, char const *const *const av)
{
	if (ac < 3)
	{
		std::cerr << "Usage: " << av[0] << " <port> <password>" << std::endl;
		return 1;
	}

	int port = atoi(av[1]);
	std::string password = av[2];

	try 
	{
		Bot bot(port, password);
		bot.run();
	}
	catch (std::exception const &e) 
	{
		std::cerr << e.what() << std::endl;
		return 1;
	}

	return 0;
}

