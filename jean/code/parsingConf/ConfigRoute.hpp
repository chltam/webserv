#pragma once

#include <iostream>
#include <map>

class ConfigRoute
{
public:
    ConfigRoute();


private:
    std::string m_root;
    std::string m_defaultFile;
    int m_allowedMethods;
    size_t m_clientBodyBufferSize;
    bool m_autoindex;
    std::map<std::string,std::string> m_cgi;
};
