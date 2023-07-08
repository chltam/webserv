#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

class Request {

    public:

        Request();
        ~Request();
        string _body;
        vector<pair<string, string>> _headerPairs;
        vector<pair<string, string>> getHeaderPairs();
        string getBody();
        string getHeaderValueFromKey(const string& key);

    private:


};