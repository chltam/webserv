#include "../includes/Server.hpp"

Server::Server(char *ConfigPath):m_Config(ConfigPath)
{

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
		pfd.push_back(fd);
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

// void Server::accepter(int index)
// {

//     m_data[index].m_newSocket = accept(m_data[index].m_sock, (struct sockaddr *)&m_data[index].m_address, (socklen_t *)&m_data[index].m_adressLen);
//     // read( m_data[index].m_newSocket,  m_data[index].m_buffer, BUFFER_SIZE );
// 	// std::cout << m_data[index].m_buffer << std::endl;
// 	pollfd	pfd;
// 	pfd.fd = m_data[index].m_newSocket;
// 	pfd.events = POLLIN;
// 	pfd.revents = 0;
// 	m_data[index].request_str.clear();
// 	while (1)
// 	{
// 		// char *buffer = new char*[BUFFER_SIZE];
// 		int	poll_result = poll(&pfd, 1, 100);
// 		if (poll_result == -1){
// 			perror("poll error");
// 			break;
// 		}
// 		else if (poll_result == 0)
// 			break;
// 		else{
// 			int bread = read( m_data[index].m_newSocket,  m_data[index].m_buffer, BUFFER_SIZE );
// 			m_data[index].m_buffer[bread] = 0;
// 			std::cout << bread << std::endl;
// 			if (bread == 0)
// 				break;
// 			else
// 				m_data[index].request_str += m_data[index].m_buffer;
// 		}
// 	}
// 	std::cout << m_data[index].request_str << std::endl;
// }

void Server::handle( int index, Socket& client_sock )
{
    RequestParser parser( client_sock.get_request_str() ); // COSMO: sometimes this string is empty with Firefox requests, should never be 
	cout << client_sock.get_request_str() << endl;

    parser.tokenizeRequest();

	// const ConfigServer& server = m_Config.getConfigServerFromRequest(parser.getHeaderValueFromKey("Host"));

	// std::cerr << server << std::endl;

    // ResponseBuilder builder( parser.getHeaderPairs(), parser.getBody() );
	ResponseBuilder builder;
    AResponse *response = builder.createResponse( *parser.createRequest(), m_Config );

    response->fillResponse();
    string respStr = response->getResponse();    // if NULL, REMOVE CLIENT

    // write to socket
    write( client_sock.get_sock_fd(), ( respStr.c_str()), respStr.length() ); // ERROR HANDLING: REMOVE CLIENT IF < 0

    close(client_sock.get_sock_fd());

    delete response;
	// delete request;
}

/*
void Server::handle( int index )
{
	// std::cout << "hererere\n" << std::endl;
    // RequestParser parser( m_data[index].request_str.c_str() );
    // parser.tokenizeRequest();

    // std::cout << m_data[index].m_buffer << std::endl;

    // ResponseBuilder builder( m_data[index].m_newSocket, parser.getHeaderPairs(), parser.getBody() );

    // AResponse *response = builder.createResponse();
	string respStr;
	if (index == 1)
		respStr = "HTTP/1.1 200 OK\r\n"
						"Content-Type: text/html\r\n"
						"Content-Length: 465\r\n"
						"\r\n"
						"<!DOCTYPE html>\r\n"
						"<html>\r\n"
						"<head>\r\n"
						"  <title>POST Request Example</title>\r\n"
						"</head>\r\n"
						"<body>\r\n"
						"  <h1>POST Request Example</h1>\r\n"
						"  <form action=\"/api/endpoint\" method=\"post\">\r\n"
						"    <label for=\"name\">Name:</label>\r\n"
						"    <input type=\"text\" id=\"name\" name=\"name\" required>\r\n"
						"    <br>\r\n"
						"    <label for=\"password\">Password:</label>\r\n"
						"    <input type=\"password\" id=\"password\" name=\"password\" required>\r\n"
						"    <br>\r\n"
						"    <button type=\"submit\">Submit</button>\r\n"
						"  </form>\r\n"
						"</body>\r\n"
						"</html>\r\n";
	else
    	respStr = "HTTP/1.1 200 OK\r\n"
                           "Content-Type: text/html\r\n"
                           "Content-Length: 178\r\n"
                           "Date: Mon, 10 Jan 2022 12:34:56 GMT\r\n"
                           "\r\n"
                           "<!DOCTYPE html>\r\n"
                           "<html>\r\n"
                           "<head>\r\n"
                           "<title>Example Website</title>\r\n"
                           "</head>\r\n"
                           "<body>\r\n"
                           "<h1>Welcome to Example Website!</h1>\r\n"
                           "<p>This is a sample HTTP response.</p>\r\n"
                           "</body>\r\n"
                           "</html>\r\n";

    write( m_data[index].m_newSocket, ( respStr.c_str()), respStr.size());
        // ERROR HANDLING: REMOVE CLIENT IF < 0


    // read any remaining data from the client
    // char buf[1024];
    // while (recv(m_data[index].m_newSocket, buf, sizeof(buf), 0) > 0) {
    //     printf("BUF: %s\n", buf);  // Keep reading until recv() returns 0, indicating the other side closed the connection
    //     bzero(buf, sizeof(buf));
    // }

    close(m_data[index].m_newSocket);
}
*/
