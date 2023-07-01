#include "server.hpp"

Server::Server(int domain, int service, int protocol, int port, u_long interface, int backlog)
{
    m_data.emplace_back(Data());
    m_data.emplace_back(Data());

    for (int i = 0; i < m_data.size(); i++)
    {
        m_data[i].m_address.sin_family = domain;
        m_data[i].m_address.sin_port = htons(port++);
        m_data[i].m_address.sin_addr.s_addr = htonl(interface);
        m_data[i].m_adressLen = sizeof( m_address );

        //establish socket
        m_data[i].m_sock = socket(domain,service,protocol);
        test_connection(m_data[i].m_sock);

        //establish network connection
        m_data[i].m_connection = bind(m_data[i].m_sock,(struct sockaddr *)&m_data[i].m_address,m_data[i].m_adressLen);
        test_connection(m_data[i].m_connection);

        m_data[i].m_backlog = backlog;

        bzero(m_data[i].m_buffer,sizeof(m_data[i].m_buffer));

    }



    // //define address structure
    // m_address.sin_family = domain;
    // m_address.sin_port = htons(port);
    // m_address.sin_addr.s_addr = htonl(interface);
    // m_adressLen = sizeof( m_address );

    // //establish socket
    // m_sock = socket(domain,service,protocol);
    // test_connection(m_sock);

    // //establish network connection
    // m_connection = bind(m_sock,(struct sockaddr *)&m_address,m_adressLen);
    // test_connection(m_connection);

    // m_backlog = backlog;

    // bzero(m_buffer,sizeof(m_buffer));




    //     //define address structure
    // m_address2.sin_family = domain;
    // m_address2.sin_port = htons(8081);
    // m_address2.sin_addr.s_addr = htonl(interface);
    // m_adressLen2 = sizeof( m_address2 );

    // //establish socket
    // m_sock2 = socket(domain,service,protocol);
    // test_connection(m_sock2);

    // //establish network connection
    // m_connection2 = bind(m_sock2,(struct sockaddr *)&m_address2,m_adressLen2);
    // test_connection(m_connection2);

    // m_backlog2 = backlog;

    // bzero(m_buffer2,sizeof(m_buffer2));
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

    for (int i = 0; i < m_data.size(); i++)
    {
        m_data[i].m_listening = listen(m_data[i].m_sock, m_data[i].m_backlog);
        test_connection(m_data[i].m_listening);
    }


    // m_listening2 = listen(m_sock2, m_backlog2);
    // test_connection(m_listening2);

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

    for (int i = 0; i < m_data.size(); i++)
    {
         std::cout << "accepting: " << i << std::endl;
         m_data[i].m_newSocket = accept( m_data[i].m_sock,(struct sockaddr *)& m_data[i].m_address,(socklen_t *)& m_data[i].m_adressLen);
         read( m_data[i].m_newSocket,  m_data[i].m_buffer, BUFFER_SIZE );
    }


    // m_newSocket = accept(m_sock,(struct sockaddr *)&m_address,(socklen_t *)&m_adressLen);
    // read( m_newSocket, m_buffer, BUFFER_SIZE );

    // m_newSocket2 = accept(m_sock2,(struct sockaddr *)&m_address2,(socklen_t *)&m_adressLen2);
    // read( m_newSocket2, m_buffer2, BUFFER_SIZE );
}

void Server::respond()
{
    write(m_data[0].m_newSocket,"wowowowoowow\n",13);
    close(m_data[0].m_newSocket);

    write(m_data[1].m_newSocket,"help\n",5);
    close(m_data[1].m_newSocket);
}

void Server::handle()
{
    //handler
    for (int i = 0; i < m_data.size(); i++)
        std::cout << m_data[i].m_buffer << std::endl;



    // std::cout << m_buffer2 << std::endl;
}
