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

#include "RequestParser.hpp"
#include "ResponseBuilder.hpp"
#include "AResponse.hpp"

#include "MetaVars.hpp"
#include "Socket.hpp"

#include "Config.hpp"

#define BUFFER_SIZE 100

using namespace std;


class Server {

    public:

		Server(char *configPath, char **envp);
        ~Server();
		void	set_server_sock(/*conf info*/);
		void	start_listening();
		void	accept_connection();



    private:


        std::vector<Socket>	_server_sock;
        Config m_Config;
		char	**_envp;
        void handle(int index, Socket& client_sock);

};

#endif
