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

        string buildResponse();
        void printHeaderInfo( void );
        void printBody( void );
        string getResponse();

    private:

        string _response;
        string _respHeader;
        string _respBody;
        
        string _reqBody;
        string _path;
        string _status;
        string _statusMsg;
        string _dateTime;
        string _serverName;
        string _contType;
        string _contLen;
        
        virtual void buildBody() const = 0;
        void buildHeader();
        void determineStatus();
        void saveDateTime();
};
