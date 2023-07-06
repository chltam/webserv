#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <poll.h>

#include <vector>
#include <algorithm>

#include "RequestParser.hpp"
#include "ResponseBuilder.hpp"
#include "AResponse.hpp"

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
        char m_buffer[BUFFER_SIZE];
};

class Server {

    public:

        Server(int domain, int service, int protocol, int port, u_long interface, int backlog);
        ~Server();

        void startListening( void );

        std::vector<Data> m_data;

    private:

        void test_connection(int);

        void accepter(int index);
        void handle(int index);
        void respond(int index);

};
