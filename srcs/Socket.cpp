#include "../includes/Socket.hpp"

Socket::Socket(){};

/**
 * @brief Socket object constructor for listening socket
 * 
 * @param ipVersion ipv4=AF_INET
 * @param service TCP=SOCK_STREAM
 * @param protocol default = 0
*/
Socket::Socket(int ipVersion, int service, int protocol)
{   
    _sock = socket(ipVersion, service, protocol);
    if (_sock < 0){
        perror("Failed to Connect...");
        exit(EXIT_FAILURE);
    }
	int optval = 1;
	if (setsockopt(_sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0) {
		std::cerr << "Failed to set SO_REUSEADDR option." << std::endl;
	}
}

/**
 * @brief Socket object constructor for client socket
 * 
 * @param listener_fd listener socket fd
*/
Socket::Socket(int listener_fd)
{
    sockaddr_in client_addr{};
    socklen_t   len = 0;
    _sock = accept(listener_fd, (sockaddr *) &client_addr, &len);
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
    if (bind(_sock, (struct sockaddr *) &sockaddr, sizeof(sockaddr)) == -1){
        perror("bind error");
        exit(EXIT_FAILURE);
    }
}

void	Socket::enable_listener()
{
	if (listen(_sock, 10) == -1){
		perror("listen error");
		exit(EXIT_FAILURE);
	}
}

void    Socket::read_sock()
{
    pollfd	pfd;
    char    buffer[BUFFER_SIZE];
	pfd.fd = _sock;
	pfd.events = POLLIN;
	pfd.revents = 0;
	_request_str.clear();
	while (1)
	{
		// char *buffer = new char*[BUFFER_SIZE];
		int	poll_result = poll(&pfd, 1, 100);
		if (poll_result == -1){
			perror("poll error");
			break;
		}
		else if (poll_result == 0)
			break;
		else{
			int bread = read( _sock,  buffer, BUFFER_SIZE );
			buffer[bread] = 0;
			if (bread == 0)
				break;
			else
				_request_str += buffer;
		}
	}
	// std::cout << _request_str << std::endl;
}

int	Socket::get_sock_fd(){
	return (_sock);
}

std::string	Socket::get_request_str(){
	return (_request_str);
}