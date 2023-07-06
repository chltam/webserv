#ifndef _SOCKET_H_
#define _SOCKET_H_

#include "Server.hpp"

class Socket {

public:
	Socket();
	Socket(int ipVersion, int service, int protocol);
	Socket(int listener_fd);
	~Socket();
	void	bind_socket(int port);
	void	read_sock();
	void	parse_request();
	void	call_cgi();
	void	parse_response();
private:
	int	_sock;
	std::string	_request_str; 
	MetaVars	_mvars;
};

#endif