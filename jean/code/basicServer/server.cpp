#include "server.hpp"

Server::Server(int domain, int service, int protocol, int port, u_long interface, int backlog)
{
    //define address structure
    m_address.sin_family = domain;
    m_address.sin_port = htons(port);
    m_address.sin_addr.s_addr = htonl(interface);
    m_adressLen = sizeof( m_address );

    //establish socket
    m_sock = socket(domain,service,protocol);
    test_connection(m_sock);

    //establish network connection
    m_connection = bind(m_sock,(struct sockaddr *)&m_address,m_adressLen);
    test_connection(m_connection);

    m_backlog = backlog;

    bzero(m_buffer,sizeof(m_buffer));
}

void Server::test_connection(int item)
{
    if(item < 0){
        perror("Failed to Connect...");
        exit(EXIT_FAILURE);
    }
}



void Server::startListening( void )
{
    m_listening = listen(m_sock, m_backlog);
    test_connection(m_listening);

    while (true)
    {
        std::cout << "========= WAITING ========" << std::endl;
        accepter();
        handle();
        respond();
        std::cout << "======== DONE =====" << std::endl;
    }
}


void Server::accepter()
{
    m_newSocket = accept(m_sock,(struct sockaddr *)&m_address,(socklen_t *)&m_adressLen);
    read( m_newSocket, m_buffer, BUFFER_SIZE );
}

void Server::respond()
{
    write(m_newSocket,"wowowowoowow\n",13);
    close(m_newSocket);
}

void Server::handle()
{
    //handler
    std::cout << m_buffer << std::endl;
}
