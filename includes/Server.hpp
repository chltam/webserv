#ifndef _SERVER_H_
#define _SERVER_H_

#pragma once

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


#include "ResponseBuilder.hpp"

#include "Request.hpp"
#include "MetaVars.hpp"
#include "Socket.hpp"
#include "Request.hpp"

#include "Config.hpp"



class Server {

    public:

		Server(char *configPath, char **envp);
        ~Server();
		void	set_server_sock(/*conf info*/);
		void	start_listening();
		void	accept_connection();


    private:

		MetaVars	_mvars;
        std::vector<Socket>	m_serverSockVec;
		std::vector<Socket>	m_clientSockVec;
		std::vector<pollfd>	m_pfdVec;
        Config m_Config;
        ResponseBuilder _builder;

        void handle(int index, Socket& client_sock);
		bool	isClientSock(int fdToCheck);
		void	removeFromVec(int fdToRemove);
		Socket&	getClientSockFromVec(int fdToFind);

};

#endif
