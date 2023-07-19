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
    ConfigServer(const std::string& defaultName, const std::string &defaultPort);
    ~ConfigServer();

    friend std::ostream& operator<< (std::ostream& stream, const ConfigServer& cs);
    const ConfigRoute* getRouteFromPath(const std::string& path) const;
public: //getters, setters
    const std::vector<std::pair<std::string,std::string>>& getPorts() const;
    const std::map<int,std::string>& getErrorPages() const;
    const std::map<std::string,ConfigRoute *>& getConfigRoutes() const;

    void AddConfigRoute(ConfigRoute * config);
    void AddServerPort(const std::string& serverName,const std::string& port);
private: 
    std::vector<std::pair<std::string,std::string>> m_ports;
    std::map<int,std::string> m_errorPages;
    std::map<std::string,ConfigRoute *> m_routes;
};
