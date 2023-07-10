#include "../includes/Server.hpp"

Server::Server(char *ConfigPath):m_Config(ConfigPath)
{
	vector<Socket> v;
	_server_sock = v;

	m_Config.printServers();
};

Server::Server(int domain, int service, int protocol, int port, u_long interface, int backlog):m_Config(NULL) {

    m_data.emplace_back(Data());
    m_data.emplace_back(Data());

    for (int i = 0; i < (int)m_data.size(); i++)
    {
        m_data[i].m_address.sin_family = domain;
        m_data[i].m_address.sin_port = htons(port++);
        m_data[i].m_address.sin_addr.s_addr = htonl(interface);
        m_data[i].m_adressLen = sizeof( m_data[i].m_address );

        //establish socket
        m_data[i].m_sock = socket(domain, service, protocol);
        test_connection(m_data[i].m_sock);

        // Enable SO_REUSEADDR option, this solves the annoying reuse issue when a server hangs on a port
        int optval = 1;
        if (setsockopt(m_data[i].m_sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0) {
            std::cerr << "Failed to set SO_REUSEADDR option." << std::endl;
        }

        // int yes = 1;
        // setsockopt(m_data[i].m_sock, SOL_SOCKET, SO_KEEPALIVE, &yes, sizeof(int));

        // For changing the keepalive interval, you need to check the specific system documentation,
        // as the socket option name can change. Here is an example for Linux:
        // int interval = 10;  // 10 seconds
        // setsockopt(m_data[i].m_sock, IPPROTO_TCP, TCP_KEEPINTVL, &interval, sizeof(int));

        //establish network connection
        m_data[i].m_connection = bind(m_data[i].m_sock,(struct sockaddr *)&m_data[i].m_address,m_data[i].m_adressLen);
        test_connection(m_data[i].m_connection);

        m_data[i].m_backlog = backlog;

        bzero( m_data[i].m_buffer, sizeof(m_data[i].m_buffer ));
    }
}

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
			sock.bind_socket(servers[i].m_ports[j].second);
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
		pfd.emplace_back(fd);
	}

	while (true)
	{
        std::cout << "========= WAITING ========" << std::endl;
		int	result = poll(pfd.data(), pfd.size(), 1000);

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

void Server::accepter(int index)
{
    m_data[index].m_newSocket = accept(m_data[index].m_sock, (struct sockaddr *)&m_data[index].m_address, (socklen_t *)&m_data[index].m_adressLen);
    // read( m_data[index].m_newSocket,  m_data[index].m_buffer, BUFFER_SIZE );
	// std::cout << m_data[index].m_buffer << std::endl;
	pollfd	pfd;
	pfd.fd = m_data[index].m_newSocket;
	pfd.events = POLLIN;
	pfd.revents = 0;
	m_data[index].request_str.clear();
	while (1)
	{
		// char *buffer = new char*[BUFFER_SIZE];
		int	poll_result = poll(&pfd, 1, 100);
		if (poll_result == -1){
			perror("poll error");
			break;
		}
		else if (poll_result == 0)
			break;
		else{
			int bread = read( m_data[index].m_newSocket,  m_data[index].m_buffer, BUFFER_SIZE );
			m_data[index].m_buffer[bread] = 0;
			std::cout << bread << std::endl;
			if (bread == 0)
				break;
			else
				m_data[index].request_str += m_data[index].m_buffer;
		}
	}
	std::cout << m_data[index].request_str << std::endl;
}

void Server::handle( int index, Socket& client_sock )
{
    RequestParser parser( client_sock.get_request_str() ); 
	// cout << client_sock.get_request_str() << endl;

    parser.tokenizeRequest();

	const ConfigServer& server = m_Config.getConfigServerFromRequest(parser.getHeaderValueFromKey("Host"));

	std::cerr << server << std::endl;

	Request request = *parser.createRequest();
	ResponseBuilder builder;
    AResponse *response = builder.createResponse( request, m_Config );

    response->fillResponse();
    string respStr = response->getResponse();    // if NULL, REMOVE CLIENT

    // write to socket
    write( client_sock.get_sock_fd(), ( respStr.c_str()), respStr.length() ); // ERROR HANDLING: REMOVE CLIENT IF < 0

    close(client_sock.get_sock_fd());

    delete response;
}
