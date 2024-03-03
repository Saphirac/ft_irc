/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcMessage.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcourtoi <mcourtoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 14:06:14 by mcourtoi          #+#    #+#             */
/*   Updated: 2024/02/26 16:58:04 by mcourtoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string>
#include <vector>

class IrcMessage
{
private:
    std::string              _prefix;
    std::string              _command;
    std::vector<std::string> _params;
    void parseSingleMessage(const std::string& msg);


public:
    IrcMessage() {}
    IrcMessage(std::string const &msg) {
        this->parseSingleMessage(msg);
    }
	~IrcMessage();

    IrcMessage(const IrcMessage& other);
    IrcMessage& operator=(const IrcMessage& other);

	std::string const        &get_prefix() const;
	std::string const        &get_command() const;
	std::vector<std::string> &get_params();

    void setPrefix(const std::string& prefix);
    void setCommand(const std::string& command);
    void setParams(const std::vector<std::string>& params);
	void addParam(const std::string& param);
	
	friend std::ostream& operator<<(std::ostream& os, const IrcMessage& msg);
};
