#include "server.hpp"

Server::Server( std::string ipAddr, int port ):
    _ipAddr( ipAddr ), _port( port ), _sockAddr() {

    _sockAddr.sin_family = AF_INET;
    _sockAddr.sin_port = htons( _port );
    _sockAddr.sin_addr.s_addr = INADDR_ANY; // getaddrinfo for other addr
    _addrLen = sizeof( _sockAddr );
    this->startServer();
};

Server::~Server( void ) {

    closeServer();
};

int Server::startServer( void ) {

    _socketFd = socket( AF_INET, SOCK_STREAM, 0); // AF_INET: for IPv4 addresses
                                                // SOCK_STREAM: for comm structure: full duplex byte streams
    if ( _socketFd < 0 ) {

        std::cout << "Could not create socket" << std::endl;
        return ( 1 );
    }

    if ( bind( _socketFd,(sockaddr *)&_sockAddr, _addrLen ) < 0) {
    
        std::cout << "Could not connect socket to address" << std::endl;
        return ( 1 );
    }

    return ( 0 );
};

void Server::acceptConnection( int &newSocket ) {

    newSocket = accept( _socketFd, (sockaddr *)&_sockAddr, 
                        &_addrLen);
    
    if (newSocket < 0) {

        std::cout << "Accepting the connection failed" << std::endl;
    }
};

int Server::startListening( void ) {

    if ( listen( _socketFd, 20 ) < 0 ) {
        
        std::cout << "Listening failed" << std::endl;
        return ( 1 );
    }
    
    std::cout << "Listening on addr: " << _ipAddr << " port: "
        << ntohs( _sockAddr.sin_port ) << std::endl;

    int readBytes;
    while ( true ) {

        acceptConnection( _newSocket );

        char buffer[BUFFER_SIZE] = {0};
        readBytes = read( _newSocket, buffer, BUFFER_SIZE );
        if ( readBytes < 0) {

            std::cout << "Did not read any bytes" << std::endl;
            return ( 1 );
        }
        std::cout << buffer << std::endl;

        sendResponse( _newSocket );
        close( _newSocket );
    }
    return ( 0 );
};

void Server::sendResponse( int &newSocket ) {

    write( newSocket, "response\n", 9 );
}

void Server::closeServer( void ) {

    close ( _socketFd );
};
