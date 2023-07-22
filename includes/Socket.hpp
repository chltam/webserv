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
#include <Config.hpp>
#include <ConfigServer.hpp>
#define BUFFER_SIZE 500

class Socket {

//server socket functions
public:
	Socket(int ipVersion, int service, int protocol);
	void	bind_socket(std::string ip, int port);
	void	enable_listener();

//client socket functions
public:
	Socket(int listener_fd);
	int					read_sock(const Config& metaConfig);
	std::string			get_request_str();
	std::vector<char>	get_request_byte();

//more generic functions, we use them mostly for client socket tho
public:
	Socket();
	~Socket();
	
	void	update_last_active_time();
	bool	is_idle(long timeout);
	int		get_sock_fd();
	long	get_last_active_time();
	bool	get_error();
	bool	get_needAuthorized();

	bool	get_sizeIssue();
	bool	get_permsIssue();
	std::string	get_host();


	void	printHeader();

private:
	std::string			_server_ip;
	int					_server_port;
	int					_sock;
	long				_last_active_time;
	std::string			_header_str; 
	std::vector<char>	_request_byte;
	std::string			_response_str;
	MetaVars			_mvars;

//header error
private:
	bool	_error;
	bool	_needAuthorize;
	bool	_sizeIssue;
	bool	_permsIssue;
	bool	_badRequest;
private:
	void	buffer_to_vec(char* buffer, int bread);
	bool	isFullHeader();
	std::string	getValueFromHeader(std::string key);
	void	checkHeaderError(const Config& metaConfig);

};

#endif