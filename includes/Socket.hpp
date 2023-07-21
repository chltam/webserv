#ifndef _SOCKET_H_
#define _SOCKET_H_

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
#include <ctime>
#include <sys/time.h>

#include <vector>
#include <algorithm>
#include <MetaVars.hpp>
#define BUFFER_SIZE 500

class Socket {

public:
	Socket();
	Socket(int ipVersion, int service, int protocol);
	Socket(int listener_fd);
	~Socket();
	void	bind_socket(std::string ip, int port);
	void	enable_listener();
	void	read_sock();
	int		read_test();
	void	update_last_active_time();
	bool	is_idle(long timeout);
	int		get_sock_fd();
	std::string	get_request_str();
	std::vector<char>	get_request_byte();
	long	get_last_active_time();

	void	updateStr();

private:
	int	_sock;
	long	_last_active_time;
	std::string	_request_str; 
	std::vector<char>	_request_byte;
	std::string	_response_str;
	MetaVars	_mvars;

private:
	void	buffer_to_vec(char* buffer, int bread);

};

#endif