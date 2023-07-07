#include "server.hpp"

Server::Server(int domain, int service, int protocol, int port, u_long interface, int backlog) {
    
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

void Server::startListening( void )
{

    for (int i = 0; i < (int)m_data.size(); i++)
    {
        m_data[i].m_listening = listen(m_data[i].m_sock, m_data[i].m_backlog);
        test_connection(m_data[i].m_listening);
    }

    std::vector<pollfd> fds(m_data.size());
    std::transform(m_data.begin(), m_data.end(), fds.begin(), [](const Data& d) {
        return pollfd{d.m_sock, POLLIN, 0};
    });

    while (true)
    {
        std::cout << "========= WAITING ========" << std::endl;
        int result = poll(fds.data(), fds.size(), 1000);
        std::cout << "right after poll()" << std::endl;

        if (result < 0) {
            perror("poll() failed");
            exit(EXIT_FAILURE); // REMOVE CLIENT ACCORDING TO EVAL SHEET
        }
        std::cerr << "RESULT = " << result << std::endl;
        for (int i = 0; i < (int)fds.size(); i++)
        {
            if (fds[i].revents & POLLIN)
            {
                std::cout << "Found proper fd at: " << i << std::endl;
                accepter(i);
                respond(i); // just our basic responses
                handle(i);
            }
        }
        std::cout << "======== DONE =====" << std::endl;
    }
}

void Server::accepter(int index)
{

    m_data[index].m_newSocket = accept(m_data[index].m_sock, (struct sockaddr *)&m_data[index].m_address, (socklen_t *)&m_data[index].m_adressLen);
    read( m_data[index].m_newSocket,  m_data[index].m_buffer, BUFFER_SIZE ); // ERROR HANDLING, REMOVE CLIENT IF < 0
}

void Server::respond(int index)
{
    (void)index;
    // if (index) { // port 8081
    //     char l[] = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: 1234\nDate: Thu, 01 Jul 2023 12:34:56 GMT\nServer: Apache\n\n<!DOCTYPE html>\n<html>\n<head>\n<title>Example Page</title>\n</head>\n<body>\n<h1>Hello, World!</h1>\n</body>\n</html>";
    //     write(m_data[index].m_newSocket,l,sizeof(l));
    // }
    // else // port 8080
    //     write(m_data[index].m_newSocket,"wtf\n",4);
}

void Server::handle( int index )
{
    RequestParser parser( m_data[index].m_buffer );
    parser.tokenizeRequest();

    ResponseBuilder builder( parser.getHeaderPairs(), parser.getBody() );
    builder.fillReqInfo();

    AResponse *response = builder.createResponse();
    
    response->fillResponse();
    string respStr = response->getResponse();    // if NULL, REMOVE CLIENT

    // write to socket
    write( m_data[index].m_newSocket, ( respStr.c_str()), respStr.length() ); // ERROR HANDLING: REMOVE CLIENT IF < 0
    
    
    // read any remaining data from the client
    // char buf[1024];
    // while (recv(m_data[index].m_newSocket, buf, sizeof(buf), 0) > 0) {
    //     printf("BUF: %s\n", buf);  // Keep reading until recv() returns 0, indicating the other side closed the connection
    //     bzero(buf, sizeof(buf));
    // }

    close(m_data[index].m_newSocket);

    delete response;
}