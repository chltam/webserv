#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include "ConfigRoute.hpp"


class ConfigServer
{
public:
    ConfigServer();
    ConfigServer(const std::string& defaultName,int defaultPort);


    friend std::ostream& operator<< (std::ostream& stream, const ConfigServer& cs);
    const ConfigRoute* getRouteFromPath(const std::string& path) const;
public: //getters, setters
    const std::vector<int>& getPorts() const;
    const std::vector<std::string>& getServerNames() const;
    const std::map<int,std::string>& getErrorPages() const;
    const std::map<std::string,ConfigRoute *>& getConfigRoutes() const;

    void AddConfigRoute(ConfigRoute * config);
    void AddServerName(const std::string& name);
    void AddServerPort(int port);
private: 
    std::vector<std::string> m_serverNames;
    std::vector<int> m_ports;
    std::map<int,std::string> m_errorPages;
    std::map<std::string,ConfigRoute *> m_routes;
};
