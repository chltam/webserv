#include "ConfigServer.hpp"

ConfigServer::ConfigServer()
{
    //setup default values
    m_root = "/default";
    m_defaultFile = "index.html";
    m_allowedMethods = METH_ALL;
    m_clientBodyBufferSize = 1000000000;
    m_autoindex = false;

}


void ConfigServer::AddRoute(std::string path, ConfigRoute &route)
{
    m_routes.emplace(path,route);
}

const ConfigRoute& ConfigServer::getRouteFromPath(const std::string& path) const
{
    return m_routes.find(path)->second;
}

std::string ConfigServer::MethodEnumToString(int val) const
{
    std::string ret;
    if(val & METH_GET)
        ret += "GET ";
    if(val & METH_POST)
        ret += "POST ";
    if(val & METH_DELETE)
        ret += "DELETE ";
    return ret;
}

std::ostream &operator<<(std::ostream &os, const ConfigServer &cs)
{
    os << "-------------Config SERVER-------------" << std::endl;
    os << "Server Name = " << cs.m_serverName << std::endl;

    os << "Root dir = " << cs.m_root << std::endl;
    os << "Default File = " << cs.m_defaultFile << std::endl;
    os << "Allowed Methods = " << cs.MethodEnumToString(cs.m_allowedMethods) << std::endl;
    os << "Client Body Buff Size = " << cs.m_clientBodyBufferSize << std::endl;
    os << "Autoindex = " << cs.m_autoindex << std::endl;

    os << "----Ports----" << std::endl;
    if(cs.m_ports.size() != 0) {
    for (int i = 0; i < cs.m_ports.size(); i++)
        os << "elem " <<i << ": " <<cs.m_ports[i].first <<" " << cs.m_ports[i].second << std::endl;
    }
    else
        os << "No Ports found" << std::endl;

    os << "----CGI----" << std::endl;
    if(cs.m_cgi.size() != 0) {
        for (int i = 0; i < cs.m_cgi.size(); i++)
            os << "elem " <<i << ": " <<cs.m_cgi[i].first <<" " << cs.m_cgi[i].second << std::endl;
    }
    else
        os << "No CGI found" << std::endl;

    os << "----Routes----" << std::endl;
    if(cs.m_routes.size() != 0){
        for (std::map<std::string,ConfigRoute>::const_iterator it = cs.m_routes.begin();it != cs.m_routes.end();it++) {
            os << "Route: " << it->first << std::endl;
            os << it->second << std::endl;
        }
    }
    else
        os << "No Routes found" << std::endl;

    return os;
}
