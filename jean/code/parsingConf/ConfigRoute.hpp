#pragma once

#include <iostream>
#include <map>

enum AllowedMethods{
    METH_NONE   = 0,
    METH_GET    = 1,
    METH_POST   = 2,
    METH_DELETE = 4,
    METH_ALL    = 7
};



class ConfigRoute
{
public:
    ConfigRoute();


    friend std::ostream& operator<< (std::ostream& os, const ConfigRoute& cr);

public: //making it public for now, otherwise its a lot of unneccesary function calls
    std::string m_root;
    std::string m_defaultFile;
    int m_allowedMethods;
    size_t m_clientBodyBufferSize;
    bool m_autoindex;
    std::map<std::string,std::string> m_cgi;

    std::string MethodEnumToString(int val) const;
};
