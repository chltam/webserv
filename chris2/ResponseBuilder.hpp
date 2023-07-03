#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <vector>

class ResponseBuilder {

    public:

        ResponseBuilder( std::vector<std::pair<std::string, std::string>> headerPairs, std::string body );

    private:

        std::vector<std::pair<std::string, std::string>> _requestHeaderPairs;
        std::string _requestBody;
        int _socket;
};
