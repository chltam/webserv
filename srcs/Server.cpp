#include "../includes/Server.hpp"

Server::Server(char *ConfigPath, char **envp):m_Config(ConfigPath), _builder(), _mvars(envp)
{
	m_Config.printServers();
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
	const std::vector<ConfigServer>& servers = m_Config.getServers();
	for(int i = 0; i < servers.size(); i++){
		for(int j = 0;j < servers[i].m_ports.size(); j++){
			Socket sock = Socket(AF_INET, SOCK_STREAM, 0);
			std::cout << "i = " << i << "Binding new Socket at: " << servers[i].m_ports[j].second  << std::endl;
			sock.bind_socket(servers[i].m_ports[j].first, servers[i].m_ports[j].second);
			m_serverSockVec.push_back(sock);

		}
	}
}


void	Server::start_listening()//need to add poll later
{
	for (int n = 0; n < m_serverSockVec.size(); n++)
	{
		m_serverSockVec[n].enable_listener();
	}

}

void	Server::accept_connection()
{
	for (int n = 0; n < m_serverSockVec.size(); n++)
	{
		// pollfd	sfd = {m_serverSockVec[n].get_sock_fd(), POLL_IN, 0};
		m_pfdVec.push_back({m_serverSockVec[n].get_sock_fd(), POLLIN, 0});
	}

	while (true)
	{
        std::cout << "========= WAITING ========" << std::endl;
		int	result = poll(m_pfdVec.data(), m_pfdVec.size(), 100);

		if (result == -1){
			perror("poll failed");
			exit(EXIT_FAILURE);
		}
		std::cout << "result = " << result << std::endl;
		for (int n = 0; n < m_pfdVec.size(); n++)
		{
			if ((m_pfdVec[n].revents & POLLIN) && !isClientSock(m_pfdVec[n].fd))
			{
				Socket	client_sock(m_pfdVec[n].fd);
				m_clientSockVec.push_back(client_sock);
				m_pfdVec.push_back({client_sock.get_sock_fd(), POLLIN , 0});
				
				client_sock.read_sock();

				handle(n, client_sock);
			}
			// if ((m_pfdVec[n].revents & POLLIN) && isClientSock(m_pfdVec[n].fd))
			// {
			// 	//read the socket
			// 	Socket&	refSock = getClientSockFromVec(m_pfdVec[n].fd);
			// 	std::cout << "here\n";
			// 	removeFromVec(refSock.get_sock_fd());

			// }
		}

	}
}

void Server::handle( int index, Socket& client_sock )
{
	// std::cout << client_sock.get_request_str() << std::endl;
	Request	request(client_sock.get_request_str());
	// request.printf_all();

	Response* resp = _builder.createNewResponse(request, m_Config, _mvars);

	std::string respString = resp->build();

    // write to socket
	write( client_sock.get_sock_fd(),  respString.c_str(), respString.length() );
	removeFromVec(client_sock.get_sock_fd());
    close(client_sock.get_sock_fd());
    delete resp;
}

bool	Server::isClientSock(int fdToCheck)
{
	for (int  n = 0; n < m_serverSockVec.size(); n++)
	{
		if (m_serverSockVec[n].get_sock_fd() == fdToCheck)
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

}