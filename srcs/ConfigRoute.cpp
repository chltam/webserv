#include "ConfigRoute.hpp"
#include "ConfigServer.hpp"

ConfigRoute::ConfigRoute()
{
    //setup default values
    m_root = "NOT DEFINED";
    m_defaultFile = "NOT DEFINED";
    m_allowedMethods = METH_NONE;
    m_clientBodyBufferSize = -1;
    m_autoindex = false;

    m_shouldRedirect = false;
}

//inheriting global configurations
ConfigRoute::ConfigRoute(const ConfigServer &server)
{
    m_root = server.m_root;
    m_defaultFile = server.m_defaultFile;
    m_allowedMethods = server.m_allowedMethods;
    m_clientBodyBufferSize = server.m_clientBodyBufferSize;
    m_autoindex = server.m_autoindex;

    if(server.m_cgi.size() != 0) {
        for (int i = 0; i < server.m_cgi.size(); i++)
            m_cgi.emplace_back(server.m_cgi[i].first,server.m_cgi[i].second);
    }

    m_shouldRedirect = false;

}

std::ostream &operator<<(std::ostream &os, const ConfigRoute &cr)
{
    std::string spaces("     ");

    os << spaces  << "--------Config ROUTE--------" << std::endl;

    os << spaces << "Root dir = " << cr.m_root << std::endl;
    os << spaces << "Default File = " << cr.m_defaultFile << std::endl;
    os << spaces << "Allowed Methods = " << cr.MethodEnumToString(cr.m_allowedMethods) << std::endl;
    os << spaces << "Client Body Buff Size = " << cr.m_clientBodyBufferSize << std::endl;
    os << spaces << "Autoindex = " << cr.m_autoindex << std::endl;

    os << spaces << spaces << "----CGI----" << std::endl;
    if(cr.m_cgi.size() != 0) {
        for (int i = 0; i < cr.m_cgi.size(); i++)
            os << spaces << spaces << "elem " <<i << ": " <<cr.m_cgi[i].first <<" " << cr.m_cgi[i].second << std::endl;
    }

    if(cr.m_shouldRedirect)
        os << spaces << spaces << "Should Redirect to = " << cr.m_redirectDir << std::endl;
    return os;
}

//duplicated function needs to be cleaned up
std::string ConfigRoute::MethodEnumToString(int val) const
{
    std::string ret;
    if(val & METH_GET)
        ret += "GET ";
    if(val & METH_POST)
        ret += "POST ";
    if(val & METH_DELETE)
        ret += "DELETE ";
    if(val > METH_ALL)
        ret+= "INVALID METHODS";
    return ret;
}
