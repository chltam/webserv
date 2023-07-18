#pragma once

#include <iostream>
#include <map>
#include <vector>
#include "../includes/webserv.hpp"

class ConfigRoute
{
public:
    ConfigRoute();
    ConfigRoute(const ConfigRoute& route);

    friend std::ostream& operator<< (std::ostream& os, const ConfigRoute& cr);

public: //getters
    const std::string& getRoot() const;
    const std::string& getDefaultFile() const;
    int getAllowedMethods() const;
    size_t getClientBodyBufferSize() const;
    bool getAutoIndex() const;
    const std::vector<std::pair<std::string,std::string>>& getCgi() const;
    const std::string& getRedirectDir() const;

private: 
    std::string m_root;
    std::string m_defaultFile;
    int m_allowedMethods;
    size_t m_clientBodyBufferSize;
    bool m_autoindex;
    std::vector<std::pair<std::string,std::string>> m_cgi;

    bool m_shouldRedirect;
    std::string m_redirectDir;

};
