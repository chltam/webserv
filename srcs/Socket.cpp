#include "../includes/Socket.hpp"

Socket::Socket(){};

/**
 * @brief Server socket (lisener) constructor
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
 * @brief Client socket constructor
 *
 * @param listener_fd listener socket fd
*/
Socket::Socket(int listener_fd, std::string	ip, int port)
{
    sockaddr_in client_addr;
    socklen_t   len = 0;

    _sock = accept(listener_fd, (sockaddr *) &client_addr, &len);
	update_last_active_time();
	_error = false;
	_needAuthorize = false;
	_sizeIssue = false;
	_permsIssue = false;
	_badRequest = false;
	_ip = ip;
	_port = port;

}

Socket::~Socket()
{

}

void    Socket::bind_socket(std::string ip, int port)
{
	_ip = ip;
	_port = port;

    sockaddr_in sockaddr;
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_port = htons(port);//ntohs() to convert it back to int
    sockaddr.sin_addr.s_addr = inet_addr(ip.c_str());

    if (bind(_sock, (struct sockaddr *) &sockaddr, sizeof(sockaddr)) == -1){
        perror("bind error");
        exit(EXIT_FAILURE);
    }
}

void	Socket::enable_listener()
{
	if (listen(_sock, 25) == -1){
		perror("listen error");
		exit(EXIT_FAILURE);
	}
}

int	Socket::read_sock(const Config& metaConfig)
{
	char	buffer[BUFFER_SIZE + 1];

	int bread = read (_sock, buffer, BUFFER_SIZE);
	PRINTVAR(bread);
	if (bread <= 0)
		return (-1);
	buffer_to_vec(buffer, bread);
	buffer[bread] = '\0';
	_header_str += buffer;
	if (isFullHeader())
		checkHeaderError(metaConfig);
	return (0);
}

void	Socket::update_last_active_time(){

    struct timeval tv;

    gettimeofday(&tv, NULL);
	_last_active_time = tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

bool	Socket::is_idle(long timeout)
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	long	now = tv.tv_sec * 1000 + tv.tv_usec / 1000;


	if (now - get_last_active_time() > timeout)
		return (true);
	else
		return (false);
}

int	Socket::get_sock_fd(){
	return (_sock);
}

std::string	Socket::get_request_str(){
	return (	std::string(_request_byte.begin(), _request_byte.end()));
}

time_t	Socket::get_last_active_time(){
	return (_last_active_time);
}

std::vector<char>	Socket::get_request_byte(){
	return (_request_byte);
}

std::string	Socket::getIp(){
	return (_ip);
}

int	Socket::getPort(){
	return (_port);
}

bool	Socket::get_error(){
	return (_error);
}

bool	Socket::get_sizeIssue(){
	return (_sizeIssue);
}

bool	Socket::get_permsIssue(){
	return (_permsIssue);
}

bool	Socket::get_needAuthorized(){
	return (_needAuthorize);
}

std::string	Socket::get_host(){
	return (_ip + ":" + toString(_port));
}

void	Socket::buffer_to_vec(char* buffer, int bread)
{
	_request_byte.insert(_request_byte.end(), buffer, buffer + bread);
}

bool	Socket::isFullHeader()
{
	size_t	pos;

	pos = _header_str.find("\r\n\r\n");
	if (pos == std::string::npos)
		return (false);
	_header_str = _header_str.substr(0, pos);
	// printHeader();
	return (true);
}

std::string	Socket::getValueFromHeader(std::string key)
{
	size_t	pos;
	size_t	endpos;

	pos = _header_str.find(key);
	if (pos == std::string::npos)
		return (std::string());
	pos += key.length() + 1;
	endpos = _header_str.find("\r\n", pos);

	return (_header_str.substr(pos, endpos - pos));

}

void	Socket::checkHeaderError(const Config& metaConfig)
{
	size_t		size;
	std::string host;
	std::string	pass;
	std::string	path;

	size = atoi(getValueFromHeader("Content-Length").c_str());
	host = get_host();
	pass = getValueFromHeader("Authorization:");

	if (_header_str.find("Authorization:") != std::string::npos)
		_needAuthorize = true;
	else
		_needAuthorize = false;

	size_t	pos;
	size_t	endpos;

	pos = _header_str.find(' ') + 1;
	endpos = _header_str.find(' ', pos);
	path = _header_str.substr(pos, endpos - pos);

	const ConfigServer*	serverConf = metaConfig.getConfigServerFromRequest(host);
	if (serverConf == NULL)
	{
		_permsIssue = true;
		_error = true;
		return ;
	}

	size_t maxBody = serverConf->getRouteFromPath(path)->getClientMaxBodySize();

	if (size > maxBody)
	{
		_sizeIssue = true;
		_error = true;
		return ;
	}
	if (pass.empty())
		return ;
	if (pass != "4242")
	{
		_permsIssue = true;
		_error = true;
	}

}

void	Socket::printHeader()
{
	std::cout << _header_str << std::endl;
}
