
void Client::part_of_each_channels(std::string quit_msg)
{
	for (std::map<ChannelName, Channel *const>::const_iterator it = this._joined_channels_by_name.begin();
	     it != this._joined_channels_by_name.end();
	     ++it)
	{
		part(this, std::vector<std::string>{it->first, quit_msg});
	}
}

// Methods //
/**
 * @brief Closes the socket of the Client instance and deletes it's associated epoll_event
 *
 * @throw ProblemWithClose() if the close() function fails.
 */
void Client::disconnect(std::string quit_msg)
{
	std::cout << "Client disconnected.\n";
	this->append_msg_out("ERROR :" + quit_msg);
	if (send(this->_socket, this->_msg_out.c_str(), this->_msg_out.size(), 0))
		throw ProblemWithSend();
	this->part_of_each_channels(quit_msg);
	if (close(this->_socket) == -1)
		throw ProblemWithClose();
	this->_socket = -1;
	delete this->_epoll_event;
}
