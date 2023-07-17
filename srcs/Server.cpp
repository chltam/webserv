#include "../includes/Server.hpp"

Server::Server(char *ConfigPath, char **envp):m_Config(ConfigPath), _builder()
{
	_envp = envp;
	m_Config.printServers();
};

Server::~Server() {};

void Server::test_connection(int item)
{
    if(item < 0){
        perror("Failed to Connect...");
        exit(EXIT_FAILURE); // we CANT USE EXIT!
    }
}

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
			cout << "i = " << i << "Binding new Socket at: " << servers[i].m_ports[j].second  << endl;
			sock.bind_socket(servers[i].m_ports[j].first, servers[i].m_ports[j].second);
			_server_sock.push_back(sock);

		}
	}
}

void	Server::start_listening()
{
	for (int n = 0; n < _server_sock.size(); n++)
	{
		_server_sock[n].enable_listener();
	}

}

void	Server::accept_connection()
{
	vector<pollfd> pfd;

	for (int n = 0; n < _server_sock.size(); n++)
	{
		pollfd	fd = {_server_sock[n].get_sock_fd(), POLL_IN, 0};
		pfd.push_back(fd);
	}

	while (true)
	{
        std::cout << "========= WAITING ========" << std::endl;
		int	result = poll(pfd.data(), pfd.size(), 100);

		if (result < 0){
			perror("poll failed");
			exit(EXIT_FAILURE); // REMOVE CLIENT ACCORDING TO EVAL SHEET
		}
		cout << "result = " << result << endl;
		for (int n = 0; n < pfd.size(); n++)
		{
			if (pfd[n].revents & POLLIN)
			{
				Socket	client_sock(pfd[n].fd);
				client_sock.read_sock();
				handle(n, client_sock);
				// cout << client_sock.get_request_str() << endl;
			}
		}
	}

}


void Server::handle( int index, Socket& client_sock )
{
	if (client_sock.get_request_str().empty() == true)
	{
   		close(client_sock.get_sock_fd());
		return ;
	}
	Request	request(client_sock.get_request_str());
	request.printf_all();


	Response* resp = _builder.createNewResponse(request,m_Config);

	std::string respString = resp->build();
	PRINT(respString);
    // write to socket
    write( client_sock.get_sock_fd(),  respString.c_str(), respString.length() ); // ERROR HANDLING: REMOVE CLIENT IF < 0

    close(client_sock.get_sock_fd());

    delete resp;
}
