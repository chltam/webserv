#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>

#include <vector>

class RequestParser {

    public:

        RequestParser( std::string buffer );
        void tokenizeRequest( void );
        std::vector<std::pair<std::string, std::string>> getHeaderPairs( void );
        std::string getBody( void );
                
    private:

        std::string _buffer;
        std::string _body;
        std::vector<std::pair<std::string, std::string>> _headerPairs;

        void printHeaderPairs( void );
        void printBody( void );
};
