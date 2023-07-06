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

        AResponse( string path, string serverName, string contType, string reqBody );
        virtual ~AResponse();

        void buildResponse();
        void printHeaderInfo();
        void printBody();
        string getResponse();

    protected:

        string _response;
        string _respHeader;
        string _respBody;
        
        string _path;
        string _serverName;
        string _contType;
        string _reqBody;
        string _status;
        string _statusMsg;
        string _dateTime;
        string _contLen;
        
        virtual void buildBody() = 0;
        void buildHeader();
        void determineStatus();
        void saveDateTime();
};
