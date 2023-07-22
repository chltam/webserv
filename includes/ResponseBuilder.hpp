#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <ctime>

#include <dirent.h>
#include <unistd.h>

#include "webserv.hpp"
#include "Config.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "MetaVars.hpp"
#include "Socket.hpp"

#define VALID_REQUEST_NUM 3

class ResponseBuilder {

    public:

        ResponseBuilder();
        ~ResponseBuilder();

        Response* createNewResponse( Request& request, const Config& config, MetaVars& m_vars, Socket& socket );
    private:

        int setResponseStatus( Request& request, const Config& config, Response& response, MetaVars& mvars, Socket& socket );
        void deleteResource(const std::string& newfullPath);
        void uploadResource(const std::string& newfullPath,std::string resourceData,std::string contentType);
		bool	isFileUploading(std::string& contentType);
		std::string	extractFileName(std::string& resoureceData);
		std::string	extractFileData(std::string& resourceData);
};
