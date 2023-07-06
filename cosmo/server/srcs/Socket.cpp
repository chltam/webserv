#include "../includes/Socket.hpp"

Socket::Socket(){};


Socket::Socket(int ipVersion, int service, int protocol)
{   
    _sock = socket(ipVersion, service, protocol);
    if (_sock < 0){
        perror("Failed to Connect...");
        exit(EXIT_FAILURE);
    }
}
Socket::Socket(int listener_fd)
{

}

Socket::~Socket()
{
	
}

void    Socket::bind_socket(int port)
{
    sockaddr_in sockaddr;
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_port = htons(port);
    sockaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    if (bind(_sock, (struct sockaddr *) &sockaddr, sizeof(sockaddr)) == -1)
        exit(EXIT_FAILURE);
}