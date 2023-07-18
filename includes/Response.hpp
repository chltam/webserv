#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>

#include <unistd.h>
#include <dirent.h>

#include "webserv.hpp"

class Response {

    public:

        Response();
        ~Response();
        void insertHeaderField(const std::string& key, const std::string& value);
        void setStatus(int);
        void setPath(const std::string& path);
        void setAutoIndex(bool val);
        void setPathFromErrorCode(int errorCode);
        void setBody(const std::string& body);
		void setCgi(bool state);
        const std::string& getResponseBody();
		bool	getCgi();

        std::string build();

    private:
    
        void headerToString();
        void bodyToString();
        std::string buildIndexPage();
        std::string createIndexingElement(const std::string& fileType,const std::string& relPath);
        std::string filetypeToString(int type);
        std::map<std::string,std::string> _headerFields;
        int _status;
		bool	_cgi;
        std::string _path;
        std::string _respHeader;
        std::string _respBody;
        bool _autoindex;

};
