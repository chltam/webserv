#pragma once

#include <iostream>
#include <sstream>
#include <string>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MAX_CONNECTIONS 20
#define BUFFER_SIZE 1024

class Server {

    public:
    
        Server( std::string ipAddr, int port );
        ~Server( void );

        int startListening( void );

    private:

        std::string _ipAddr;
        int _port;
        int _socketFd;
        int _newSocket;
        struct sockaddr_in _sockAddr;
        unsigned int _addrLen;

        int startServer( void );
        void acceptConnection( int &newSocket);
        void sendResponse( int &newSocket );
        void closeServer( void );
};
