#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <ctime>

#include <unistd.h>

class ResponseBuilder {

    public:

        ResponseBuilder( int socket,
            std::vector<std::pair<std::string, std::string>> headerPairs, std::string body );

        void printHeaderInfo( void );
        void printBody( void );
        void buildResponse( void );
        void writeToSocket( void );

    private:

        std::vector<std::pair<std::string, std::string>> _reqHeaderPairs;
        std::string _reqBody;
        int _socket;

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

        void buildHeader( void );
        void determineStatus( void );
        void saveDateTime( void );
        void buildBody( void );
};
