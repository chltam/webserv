#include "ConfigServer.hpp"



ConfigServer::ConfigServer()
{

}

ConfigServer::ConfigServer(std::string data)
{
    
}

void ConfigServer::AddRoute(std::string path, ConfigRoute &route)
{
    m_routes.emplace(path,route);
}

std::ostream &operator<<(std::ostream &os, const ConfigServer &cs)
{
    os << "-------------Config SERVER-------------" << std::endl;


    return os;
}
