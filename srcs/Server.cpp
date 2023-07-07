#include "../includes/Server.hpp"

Server::Server(int temp/*config info*/)
{
	for (int i = 0; i < 2; i++){
		Socket sock = Socket(AF_INET, SOCK_STREAM, 0);
		sock.bind_socket(8080 + i);
		_server_sock.push_back(sock);
	}
};

Server::~Server() {};

// void Server::test_connection(int item)
// {
//     if(item < 0){
//         perror("Failed to Connect...");
//         exit(EXIT_FAILURE); // we CANT USE EXIT!
//     }
// }

/**
 * @brief setup the vector of server socket, according to config info
 * 
 * @param //config object?
*/
// void	Server::set_server_sock(/*config info*/)
// {
// 	//temp
// 	for (int i = 0; i < 2; i++){
// 		Socket sock = Socket(AF_INET, SOCK_STREAM, 0);
// 		cout << "here2 " << i  << endl;
// 		sock.bind_socket(8080 + i);
// 		_server_sock.push_back(sock);
// 	}

	
// }

void	Server::start_listening()//need to add poll later
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

// void Server::startListening( void )
// {

//     for (int i = 0; i < (int)m_data.size(); i++)
//     {
//         m_data[i].m_listening = listen(m_data[i].m_sock, m_data[i].m_backlog);
//         test_connection(m_data[i].m_listening);
//     }

//     std::vector<pollfd> fds(m_data.size());
// 	for (int n = 0; n < m_data.size(); n++)
// 	{
// 		fds[n].fd = m_data[n].m_sock;
// 		fds[n].events = POLLIN;
// 		fds[n].revents = 0;
// 	}

//     while (true)
//     {
//         std::cout << "========= WAITING ========" << std::endl;
//         int result = poll(fds.data(), fds.size(), 1000);
//         std::cout << "right after poll()" << std::endl;

//         if (result < 0) {
//             perror("poll() failed");
//             exit(EXIT_FAILURE); // REMOVE CLIENT ACCORDING TO EVAL SHEET
//         }
//         std::cerr << "RESULT = " << result << std::endl;
//         for (int i = 0; i < (int)fds.size(); i++)
//         {
//             if (fds[i].revents & POLLIN)
//             {
//                 std::cout << "Found proper fd at: " << i << std::endl;
//                 accepter(i);
//                 respond(i); // just our basic responses
//                 handle(i);
//             }
//         }
//         std::cout << "======== DONE =====" << std::endl;
//     }
// }

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

    RequestParser parser( client_sock.get_request_str() );
    parser.tokenizeRequest();

    ResponseBuilder builder( parser.getHeaderPairs(), parser.getBody() );
    builder.fillReqInfo();

    AResponse *response = builder.createResponse();
    
    response->fillResponse();
    string respStr = response->getResponse();    // if NULL, REMOVE CLIENT

    // write to socket
    write( client_sock.get_sock_fd(), ( respStr.c_str()), respStr.length() ); // ERROR HANDLING: REMOVE CLIENT IF < 0

    close(client_sock.get_sock_fd());

    delete response;
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