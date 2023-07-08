#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <ctime>

#include <dirent.h>
#include <unistd.h>

#include "Config.hpp"
#include "RequestParser.hpp"
#include "AResponse.hpp"
#include "GetResponse.hpp"
#include "PostResponse.hpp"
#include "DeleteResponse.hpp"
#include "ErrorResponse.hpp"
#include "Request.hpp"

#define VALID_REQUEST_NUM 3

using namespace std;

class ResponseBuilder {

    public:

        // ResponseBuilder( vector<pair<string, string>> headerPairs, string body );
        ResponseBuilder( );
        ~ResponseBuilder();

        AResponse* createResponse(Request& request, const Config& config);

    private:
        void fillReqInfo(Request& request, const Config& config);

        // vector<pair<string, string>> _reqHeaderPairs;
        // string _reqBody;
        // Config _config;
        // RequestParser _request;

        string _reqType;
        string _path;
        string _status;
        string _statusMsg;
        string _dateTime;
        string _serverName;
        string _contType;
        string _contLen;

        string _respHeader;
        string _respBody;
};
