#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>

#include <sys/socket.h>
#include <netinet/in.h>

#define BUFFER_SIZE 1024

class Server {

public:
    Server(int domain, int service, int protocol, int port, u_long interface, int backlog);

    void startListening( void );


private:
    int m_sock;
    struct sockaddr_in m_address;
    int m_adressLen;
    int m_connection;
    int m_backlog;
    int m_listening;

    int m_newSocket;
    char m_buffer[BUFFER_SIZE];


    void test_connection(int);

    void accepter();
    void handle();
    void respond();

};
