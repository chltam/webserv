#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>

#include <vector>

using namespace std;

class RequestParser {

    public:

        RequestParser( string sockBuffer );
        void tokenizeRequest();
        vector<pair<string, string> > getHeaderPairs();
        std::string getHeaderValueFromKey(const std::string& key);
        string getBody();

    private:

        string _buffer;
        string _body;
        vector<pair<string, string> > _headerPairs;

        void printHeaderPairs();
        void printBody();
};
