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
    m_redirectDir= "";
}

//inheriting global configurations
ConfigRoute::ConfigRoute(const ConfigRoute &route)
{
    m_root = route.m_root;
    m_defaultFile = route.m_defaultFile;
    m_allowedMethods = route.m_allowedMethods;
    m_clientBodyBufferSize = route.m_clientBodyBufferSize;
    m_autoindex = route.m_autoindex;

    if(route.m_cgi.size() != 0) {
        for (int i = 0; i < route.m_cgi.size(); i++)
            m_cgi.emplace_back(route.m_cgi[i].first,route.m_cgi[i].second);
    }

    m_shouldRedirect = false;

}

const std::string &ConfigRoute::getRoot() const
{
    return m_root;
}

const std::string &ConfigRoute::getDefaultFile() const
{
    return m_defaultFile;
}

int ConfigRoute::getAllowedMethods() const
{
    return m_allowedMethods;
}

size_t ConfigRoute::getClientBodyBufferSize() const
{
    return m_clientBodyBufferSize;
}

bool ConfigRoute::getAutoIndex() const
{
    return m_autoindex;
}

const std::vector<std::pair<std::string, std::string>> &ConfigRoute::getCgi() const
{
    return m_cgi;
}

const std::string &ConfigRoute::getRedirectDir() const
{
    return m_redirectDir;
}

std::ostream &operator<<(std::ostream &os, const ConfigRoute &cr)
{
    std::string spaces("     ");

    os << spaces  << "--------Config ROUTE--------" << std::endl;

    os << spaces << "Root dir = " << cr.m_root << std::endl;
    os << spaces << "Default File = " << cr.m_defaultFile << std::endl;
    os << spaces << "Allowed Methods = " << MethodEnumToString(cr.m_allowedMethods) << std::endl;
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
