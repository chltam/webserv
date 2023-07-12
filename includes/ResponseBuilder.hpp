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
#include "AResponse.hpp"
#include "GetResponse.hpp"
#include "PostResponse.hpp"
#include "DeleteResponse.hpp"
#include "ErrorResponse.hpp"
#include "RedirResponse.hpp"
#include "Request.hpp"

#define VALID_REQUEST_NUM 3

class ResponseBuilder {

    public:

        ResponseBuilder();
        ~ResponseBuilder();

        AResponse* createResponse( Request& request, const Config& config );

    private:

        int fillReqInfo( Request& request, const Config& config );
        int buildPath( Request& request, const Config& config );

        std::string _reqType;
        std::string _path;
        std::string _status;
        std::string _statusMsg;
        std::string _dateTime;
        std::string _serverName;
        std::string _contType;
        std::string _contLen;

        std::string _respHeader;
        std::string _respBody;
        std::map<std::string,std::string>_header;

        std::string headerToString();

};
