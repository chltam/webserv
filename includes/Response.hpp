#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>

#include <unistd.h>
#include "webserv.hpp"

class Response {

    public:

        Response();
        ~Response();

        std::map<std::string,std::string> _headerFields;
        void setStatus(int);
        void setPath(const std::string& path);
        const std::string& getResponseBody();
        std::string build();
    private:    
        
        std::string _respHeader;
        int _status;
        std::string _path;
        std::string _respBody;
        void headerToString();
        void bodyToString();

};
