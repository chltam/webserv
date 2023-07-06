#pragma once

#include <iostream>
#include <vector>
#include <map>
#include "ConfigRoute.hpp"


class ConfigServer
{
public:
    ConfigServer();
    ConfigServer(std::string data);


    void AddRoute(std::string path, ConfigRoute& route);
    friend std::ostream& operator<< (std::ostream& stream, const ConfigServer& cs);

public: //getters, setters



public: //making it public for now since it will be used a lot
    std::string m_serverName;
    std::vector<std::pair<std::string,std::string>> m_ports;
    std::map<std::string,ConfigRoute> m_routes;
    std::map<int,std::string> m_errorPages;

    std::string m_root;
    std::string m_defaultFile;
    int m_allowedMethods;
    size_t m_clientBodyBufferSize;
    bool m_autoindex;
    std::map<std::string,std::string> m_cgi;

    std::string MethodEnumToString(int val) const;

};
