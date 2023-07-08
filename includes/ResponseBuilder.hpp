#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <ctime>

#include <unistd.h>

#include "AResponse.hpp"
#include "GetResponse.hpp"
#include "PostResponse.hpp"
#include "DeleteResponse.hpp"
#include "ErrorResponse.hpp"

#define VALID_REQUEST_NUM 3

using namespace std;

class ResponseBuilder {

    public:

        ResponseBuilder( vector<pair<string, string> > headerPairs, string body );

        ~ResponseBuilder();

        void fillReqInfo();
        AResponse* createResponse();

    private:

        vector<pair<string, string> > _reqHeaderPairs;
        string _reqBody;

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
