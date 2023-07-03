#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unistd.h>

#include <vector>

class RequestParser {

    public:

        RequestParser( std::string buffer, int socket );
        void tokenizeRequest( void );
        // std::vector<std::pair<std::string, std::string>> getReqInfo( void );

    private:

        std::string _buffer;
        int _socket;

        std::vector<std::pair<std::string, std::string>> _reqPairs;

};
