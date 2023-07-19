#include "ConfigServer.hpp"
#include "../includes/webserv.hpp"

ConfigServer::ConfigServer()
{

}


//find the best matching path
// path = /whatever/anotherdir/newdir
//  /
//  /whatever/
//  /whatever/anotherdir/
//  /whatever/anotherdir/newdir
const ConfigRoute* ConfigServer::getRouteFromPath(const std::string& path) const
{
    const ConfigRoute* route = m_routes.find("/")->second; //guaranteed to be in there
    std::map<std::string,ConfigRoute*>::const_iterator it;
    std::string tempPath("/");

    size_t prev = 0;
    while(true){ //sketchy! might find a better way out
        it = m_routes.find(tempPath);
        if(it == m_routes.end()){
            //std::cout << "Couldn't find:" << tempPath << std::endl;
            break;
        }
        else{
            //std::cout << "Found route with path [" << tempPath << "]" << std::endl;
            route = m_routes.find(tempPath)->second;
        }
        prev = path.find("/",prev+1);
        PRINTVAR(prev);
        if(prev != std::string::npos){
            tempPath = path.substr(0,prev);
        }
        else
             tempPath = path.substr(0);
        if(tempPath.empty()){
            break;
        }
    }
    PRINT("Final CONFIG");
    PRINTVAR(*route);

    return route;
}

std::ostream &operator<<(std::ostream &os, const ConfigServer &cs)
{
    os << "-------------Config SERVER-------------" << std::endl;

    os << "----Ports----" << std::endl;
    if(cs.m_ports.size() != 0) {
        for (int i = 0; i < cs.m_ports.size(); i++)
            os << "elem " << i << ": " << cs.m_ports[i]  << std::endl;
    }
    else
        os << "No Ports found" << std::endl;

    os << "----Server Names----" << std::endl;
    if(cs.m_serverNames.size() != 0) {
        for (int i = 0; i < cs.m_serverNames.size(); i++)
            os << "elem " << i << ": " << cs.m_serverNames[i]  << std::endl;
    }
    else
        os << "No Ports found" << std::endl;
    os << "----Routes----" << std::endl;
    if(cs.m_routes.size() != 0){
        for (std::map<std::string,ConfigRoute *>::const_iterator it = cs.m_routes.begin();it != cs.m_routes.end();it++) {
            os << "Route: " << it->first << std::endl;
            os << *it->second << std::endl;
        }
    }
    else
        os << "No Routes found" << std::endl;

    return os;
}

ConfigServer::ConfigServer(const std::string &defaultName, int defaultPort)
{
    m_ports.emplace_back(defaultPort);
    m_serverNames.emplace_back(defaultName);
}

const std::vector<int> &ConfigServer::getPorts() const
{
    return m_ports;
}

const std::vector<std::string> &ConfigServer::getServerNames() const
{
    return m_serverNames;
}

const std::map<int, std::string> &ConfigServer::getErrorPages() const
{
    return m_errorPages;
}

const std::map<std::string, ConfigRoute*> &ConfigServer::getConfigRoutes() const
{
    return m_routes;
}

void ConfigServer::AddConfigRoute(ConfigRoute *config)
{
    PRINT("        --ADDING CONFIG TO MAP");
    PRINTVAR(config->getPath());
    m_routes.emplace(config->getPath(),config);
}

void ConfigServer::AddServerName(const std::string &name)
{
    if(m_serverNames.size() == 0)
        m_serverNames.emplace_back(name);
    else if(std::find(m_serverNames.begin(),m_serverNames.end(),name) == m_serverNames.end()){
        m_serverNames.emplace_back(name);
    }
    else{
        std::cout << "Tried adding: [" <<name << "] but name already exists" << std::endl;
    }

    
}

void ConfigServer::AddServerPort(int port)
{
}
