#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>

#include <vector>
#include <algorithm>

#define BUFFER_SIZE 1024

class Data {
public:
    int m_sock;
    struct sockaddr_in m_address;
    int m_adressLen;
    int m_connection;
    int m_backlog;
    int m_listening;

    int m_newSocket;
    std::string m_buffer;
};

class Server {

public:
    Server(int domain, int service, int protocol, int port, u_long interface, int backlog);

    void startListening( void );

    std::vector<Data> m_data;

private:

    int m_sock;
    struct sockaddr_in m_address;
    int m_adressLen;
    int m_connection;
    int m_backlog;
    int m_listening;

    int m_newSocket;
    char m_buffer[BUFFER_SIZE];

    int m_sock2;
    struct sockaddr_in m_address2;
    int m_adressLen2;
    int m_connection2;
    int m_backlog2;
    int m_listening2;

    int m_newSocket2;
    char m_buffer2[BUFFER_SIZE];


    void test_connection(int);

    void accepter(int index);
    void handle(int index);
    void respond(int index);

};
