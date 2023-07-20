#include "ConfigServer.hpp"
#include "webserv.hpp"

ConfigServer::ConfigServer()
{
    //setup default values
    m_root = "/files";
    m_defaultFile = "index.html";
    m_allowedMethods = METH_ALL;
    m_clientBodyBufferSize = 1000000000;
    m_autoindex = false;

}


void ConfigServer::AddRoute(std::string path, ConfigRoute &route)
{
    m_routes.emplace(path,route);
}

//find the best matching path
// path = /whatever/anotherdir/newdir
//  /
//  /whatever/
//  /whatever/anotherdir/
//  /whatever/anotherdir/newdir
const ConfigRoute* ConfigServer::getRouteFromPath(const std::string& path) const
{
    const ConfigRoute* route = &(m_routes.find("/")->second); //guaranteed to be in there
    std::map<std::string,ConfigRoute>::const_iterator it;
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
            route = &(m_routes.find(tempPath)->second);
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
    }
    // PRINT("Final CONFIG");
    // PRINTVAR(*route);

    return route;
}

std::ostream &operator<<(std::ostream &os, const ConfigServer &cs)
{
    os << "-------------Config SERVER-------------" << std::endl;
    os << "Server Name = " << cs.m_serverName << std::endl;

    os << "Root dir = " << cs.m_root << std::endl;
    os << "Default File = " << cs.m_defaultFile << std::endl;
    os << "Allowed Methods = " << MethodEnumToString(cs.m_allowedMethods) << std::endl;
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
