#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <vector>

#include "Request.hpp"

using namespace std;

class RequestParser {

    public:

        RequestParser( string sockBuffer );
        void tokenizeRequest();
        Request* createRequest();
        string getHeaderValueFromKey(const string& key);
       
    private:

        string _buffer;
        string _body;
        vector<pair<string, string>> _headerPairs;

        void printHeaderPairs();
        void printBody();
};
