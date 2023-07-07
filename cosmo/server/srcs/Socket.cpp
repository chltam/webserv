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
    sockaddr_in client_addr{};
    socklen_t   len;
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
			std::cout << bread << std::endl;
			if (bread == 0)
				break;
			else
				_request_str += buffer;
		}
	}
	std::cout << _request_str << std::endl;
}