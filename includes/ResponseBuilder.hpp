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

#define VALID_REQUEST_NUM 3

class ResponseBuilder {

    public:

        ResponseBuilder();
        ~ResponseBuilder();

        Response* createNewResponse( Request& request, const Config& config );
    private:

        int setResponseStatus( Request& request, const Config& config, Response& response  );
        void deleteResource(const std::string& newfullPath);
        void uploadResource(const std::string& newfullPath,std::string resourceData);
};
