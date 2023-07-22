#include "../includes/Server.hpp"

Server::Server(char *ConfigPath, char **envp):m_Config(ConfigPath), _builder(), _mvars(envp)
{
	PRINT_LOG(m_Config);
};

Server::~Server() {};

/**
 * @brief setup the vector of server socket, according to config info
 *
 * @param //config object?
*/
void	Server::set_server_sock(/*config info*/)
{
	//slightly less temp, how do we get the right server object from the socket in question?
	const std::vector<ConfigServer*>& servers = m_Config.m_servers;
	for(int i = 0; i < servers.size(); i++){
		const std::vector<std::pair<std::string,std::string>>& ports = m_Config.m_servers[i]->getPorts();
		for(int j = 0;j < ports.size(); j++){
			Socket sock = Socket(AF_INET, SOCK_STREAM, 0);
			PRINT_LOG("server [",i,"] Binding socket at:",ports[j].second );
			sock.bind_socket(ports[j].first, std::atoi(ports[j].second.c_str()));
			_server_sock.push_back(sock);

		}
	}
}


void	Server::start_listening()//need to add poll later
{
	for (int n = 0; n < _server_sock.size(); n++)
	{
		_server_sock[n].enable_listener();
	}

}

void	Server::accept_connection()
{
	for (int n = 0; n < _server_sock.size(); n++)
	{
		pollfd	sfd = {_server_sock[n].get_sock_fd(), POLL_IN, 0};
		m_pfdVec.push_back(sfd);
	}

	while (true)
	{
        // std::cout << "========= WAITING ========" << std::endl;
		int	result = poll(m_pfdVec.data(), m_pfdVec.size(), 100);

		if (result == -1){
			perror("poll failed");
			exit(EXIT_FAILURE);
		}
		// std::cout << "result = " << result << std::endl;
		for (int n = 0; n < m_pfdVec.size(); n++)
		{
			if ((m_pfdVec[n].revents & POLLIN) && !isClientSock(m_pfdVec[n].fd))
			{
				std::cout << "connection established" << std::endl;
				Socket	client_sock(m_pfdVec[n].fd);
				m_clientSockVec.push_back(client_sock);
				pollfd	cfd = {client_sock.get_sock_fd(), POLLIN | POLLOUT , 0};
				m_pfdVec.push_back(cfd);
			}

			else if ((m_pfdVec[n].revents & POLLIN) && isClientSock(m_pfdVec[n].fd))
			{
				std::cout << "reading a request" << std::endl;
				//read the socket
				Socket&	readSock = getClientSockFromVec(m_pfdVec[n].fd);
				readSock.update_last_active_time();
				if (readSock.read_sock() == -1)
				{
					close(m_pfdVec[n].fd);
					removeFromVec(m_pfdVec[n].fd);

				}
			}

			else if ((m_pfdVec[n].revents & POLLOUT) && getClientSockFromVec(m_pfdVec[n].fd).is_idle(200))
			{
				std::cout << "ready to handle request" << std::endl;

				Socket&	writeSock = getClientSockFromVec(m_pfdVec[n].fd);
				// writeSock.updateStr();
					handle(writeSock);
			}
		}

	}
}

void Server::handle( Socket& client_sock )
{
	// std::cout << client_sock.get_request_str() << std::endl;
	Request	request(client_sock.get_request_str());
	PRINTVAR(client_sock.get_request_str());
	// request.printf_all();

	Response* resp = _builder.createNewResponse(request, m_Config, _mvars);

	std::string respString = resp->build();

    // write to socket
	write( client_sock.get_sock_fd(),  respString.c_str(), respString.length() );
    close(client_sock.get_sock_fd());
	removeFromVec(client_sock.get_sock_fd());
    delete resp;
}

bool	Server::isClientSock(int fdToCheck)
{
	for (int  n = 0; n < _server_sock.size(); n++)
	{
		if (_server_sock[n].get_sock_fd() == fdToCheck)
			return (false);
	}
	return (true);
}

/**
 * @brief remove the object associated to the target fd from socket vector and pfd vector
*/
void	Server::removeFromVec(int fdToRemove)
{
	for (std::vector<pollfd>::iterator it = m_pfdVec.begin(); it != m_pfdVec.end(); it++)
	{
		if (it->fd == fdToRemove)
		{
			m_pfdVec.erase(it);
			break ;
		}
	}

	for (std::vector<Socket>::iterator it = m_clientSockVec.begin(); it != m_clientSockVec.end(); it++)
	{
		if (it->get_sock_fd() == fdToRemove)
		{
			m_clientSockVec.erase(it);
			break ;
		}
	}

}

Socket&	Server::getClientSockFromVec(int fdToFind)
{
	for (int n = 0; n < m_clientSockVec.size(); n++)
	{
		if (m_clientSockVec[n].get_sock_fd() == fdToFind)
			return (m_clientSockVec[n]);
	}
	throw "blablablabla!";
}
