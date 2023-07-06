#include "ConfigRoute.hpp"

ConfigRoute::ConfigRoute()
{
    //setup default values
    m_root = "NOT DEFINED";
    m_defaultFile = "NOT DEFINED";
    m_allowedMethods = METH_NONE;
    m_clientBodyBufferSize = -1;
    m_autoindex = false;
}

std::ostream &operator<<(std::ostream &os, const ConfigRoute &cr)
{
    os << "-------------Config SERVER-------------" << std::endl;

    os << "Root dir = " << cr.m_root << std::endl;
    os << "Default File = " << cr.m_defaultFile << std::endl;
    os << "Allowed Methods = " << cr.MethodEnumToString(cr.m_allowedMethods) << std::endl;
    os << "Client Body Buff Size = " << cr.m_clientBodyBufferSize << std::endl;
    os << "Autoindex = " << cr.m_autoindex << std::endl;
    os << "----Ports----" << std::endl;

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
