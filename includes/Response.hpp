#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>

#include <unistd.h>
#include "webserv.hpp"

class Response {

    public:

        Response();
        ~Response();
        void insertHeaderField(const std::string& key, const std::string& value);
        void setStatus(int);
        void setPath(const std::string& path);
        const std::string& getResponseBody();

        std::string build();
    private:

        std::string _respHeader;
        std::map<std::string,std::string> _headerFields;
        int _status;
        std::string _path;
        std::string _respBody;
        void headerToString();
        void bodyToString();

};
