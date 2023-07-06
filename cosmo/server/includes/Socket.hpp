#ifndef _SOCKET_H_
#define _SOCKET_H_

#include "Server.hpp"

class Socket {

public:
	Socket();
	Socket(int listener_fd);
	~Socket();
	void	read_sock();
	void	parse_request();
	void	call_cgi();
	void	parse_response();
private:
	int	_sock_fd;
	std::string	_request_str; 
	MetaVars	_mvars;
};

#endif