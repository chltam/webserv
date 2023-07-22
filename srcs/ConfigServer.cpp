#include "ConfigServer.hpp"
#include "../includes/webserv.hpp"

ConfigServer::ConfigServer()
{
    //setting up default error pages
    std::string dir = "./errorpages/";
    m_errorPages[403] = dir + "error403.html";
    m_errorPages[404] = dir + "error404.html";
    m_errorPages[408] = dir + "error408.html";
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
        // PRINTVAR(prev);
        if(prev != std::string::npos){
            tempPath = path.substr(0,prev);
        }
        else
             tempPath = path.substr(0);
        if(tempPath.empty()){
            break;
        }
    PRINT_WARNING(tempPath.c_str());
    }
    // PRINT("Final CONFIG");
    // PRINTVAR(*route);

    return route;
}

std::ostream &operator<<(std::ostream &os, const ConfigServer &cs)
{
    os << "-------------Config SERVER-------------" << std::endl;

    os << "----Ports----" << std::endl;
    if(cs.m_ports.size() != 0) {
        for (int i = 0; i < cs.m_ports.size(); i++)
            os << "elem " << i << ": " << cs.m_ports[i].first << " " << cs.m_ports[i].second << std::endl;
    }
    else
        os << "No Ports found" << std::endl;

    os << "----Error_Pages----" << std::endl;
    if(cs.m_errorPages.size() != 0){
        for (std::map<int,std::string>::const_iterator it = cs.m_errorPages.begin();it != cs.m_errorPages.end();it++) {
            os << it->first  << " = " << it->second << std::endl;
        }
    }
    else
        os << "No Routes found" << std::endl;

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

ConfigServer::ConfigServer(const std::string &defaultName, const std::string &defaultPort)
{
    m_ports.emplace_back(std::pair<std::string,std::string>(defaultName,defaultPort));
}

ConfigServer::~ConfigServer()
{

    for (std::map<std::string,ConfigRoute*>::const_iterator it = m_routes.begin(); it != m_routes.end(); it++){
        delete it->second;
    }


}

const std::vector<std::pair<std::string,std::string>>& ConfigServer::getPorts() const
{
    return m_ports;
}

const std::map<int, std::string> &ConfigServer::getErrorPages() const
{
    return m_errorPages;
}

std::string ConfigServer::getErrorPageFromCode(int errorCode) const
{
    std::map<int,std::string>::const_iterator it = m_errorPages.find(errorCode);
    if(it == m_errorPages.end()) {
        PRINT_WARNING("WARNING, given errorfilepage was not found in server config array");
        return "FILE NOT FOUND IN ARRAY";
    }
    return it->second;
}

const std::map<std::string, ConfigRoute*> &ConfigServer::getConfigRoutes() const
{
    return m_routes;
}

void ConfigServer::AddConfigRoute(ConfigRoute *config)
{
    PRINT("        --ADDING CONFIG TO MAP");
    PRINTVAR(config->getPath());
    m_routes[config->getPath()] = config;
    // m_routes.emplace(config->getPath(),config);
}

void ConfigServer::AddServerPort(const std::string& serverName,const std::string& port)
{
    m_ports.emplace_back(std::pair<std::string,std::string>(serverName,port));
}

void ConfigServer::AddErrorPage(const std::string &key, const std::string &value)
{
    if(key.find_first_not_of("0123456789") != std::string::npos){
        PRINT_WARNING("WARNING, key of error_page does not contain only digits");
        PRINT_WARNING(key.c_str());
        return ;
    }

    int ret = ValidatePath(value);
    if(ret != S_IFREG){
        PRINT_WARNING("WARNING, path of error_page is not a valid file");
        PRINT_WARNING(value.c_str());
        return ;
    }
    int errorCode = std::atoi(key.c_str());

    m_errorPages[errorCode] = value;
}
