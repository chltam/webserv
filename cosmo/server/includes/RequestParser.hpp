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
        void tokenizeRequest();
        std::vector<std::pair<std::string, std::string>> getHeaderPairs();
        std::string getBody();
                
    private:

        std::string _buffer;
        std::string _body;
        std::vector<std::pair<std::string, std::string>> _headerPairs;

        void printHeaderPairs();
        void printBody();
};
