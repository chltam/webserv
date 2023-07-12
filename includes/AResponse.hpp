#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <ctime>

#include <unistd.h>

using namespace std;

class AResponse {

    public:

        AResponse(string path, string _header, string reqBody );
        virtual ~AResponse();

        void fillResponse();
        void printHeaderInfo();
        void printBody();
        string getResponse();
        int getExecResult();
        string getStatus();

    protected:

        string _response;
        string _respHeader;
        string _respBody;

        string _path;
        string _serverName;
        string _contType;
        string _reqBody;
        string _status;
        string _dateTime;
        string _contLen;
        int    _execResult;

        string _header;

        virtual int exec() = 0;
};
