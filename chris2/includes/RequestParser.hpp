#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>

#include <vector>

using namespace std;

class RequestParser {

    public:

        RequestParser( std::string sockBuffer );
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
