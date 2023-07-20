#pragma once

#include <iostream>
#include <map>
#include <vector>
#include "webserv.hpp"

class ConfigServer;


class ConfigRoute
{
public:
    ConfigRoute();
    ConfigRoute(const ConfigServer& server);

    friend std::ostream& operator<< (std::ostream& os, const ConfigRoute& cr);

public: //making it public for now, otherwise its a lot of unneccesary function calls
    std::string m_root;
    std::string m_defaultFile;
    int m_allowedMethods;
    size_t m_clientBodyBufferSize;
    bool m_autoindex;
    std::vector<std::pair<std::string,std::string> > m_cgi;

    bool m_shouldRedirect;
    std::string m_redirectDir;

};
