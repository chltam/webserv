#ifndef _SOCKET_H_
#define _SOCKET_H_

// #include "Server.hpp"
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
#include <arpa/inet.h>
#include <poll.h>

#include <vector>
#include <algorithm>
#include <MetaVars.hpp>
#define BUFFER_SIZE 100

class Socket {

public:
	Socket();
	Socket(int ipVersion, int service, int protocol);
	Socket(int listener_fd);
	~Socket();
	void	bind_socket(std::string ip, int port);
	void	enable_listener();
	void	read_sock();
	void	parse_request();
	void	call_cgi();
	void	parse_response();
	int		get_sock_fd();
	string	get_request_str();
private:
	int	_sock;
	std::string	_request_str; 
	std::string	_response_str;
	MetaVars	_mvars;
};

#endif