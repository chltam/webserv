#include "ConfigRoute.hpp"
#include "ConfigServer.hpp"

ConfigRoute::ConfigRoute()
{
    //setup default values
    m_path = "NOT DEFINED";
    m_root = "NOT DEFINED";
    m_allowedMethods = METH_NONE;
    m_clientMaxBodySize = -1;
    m_autoindex = false;

    m_redirectDir= "";
    m_clearIndexFiles = true;
}

ConfigRoute::ConfigRoute(const std::string& path,const std::string &root, const std::string &defaultFile, int allowedMethods, size_t bodyBufferSize, bool autoIndex, const std::string &redirectDir)
{
    m_path = path;
    m_root = root;
    m_indexFiles.push_back(defaultFile);
    m_allowedMethods = allowedMethods;
    m_clientMaxBodySize = bodyBufferSize;
    m_autoindex = autoIndex;

    m_redirectDir= redirectDir;
    m_clearIndexFiles = true;
}

// inheriting configurations except for redirect
ConfigRoute::ConfigRoute(const ConfigRoute &route)
{
    m_path = route.getPath();
    m_root = route.getRoot();
    m_allowedMethods = route.getAllowedMethods();
    m_clientMaxBodySize = route.getClientMaxBodySize();
    m_autoindex = route.getAutoIndex();

    if(route.m_cgi.size() != 0) {
        for (size_t i = 0; i < route.m_cgi.size(); i++)
            m_cgi.push_back(std::make_pair( route.m_cgi[i].first,route.m_cgi[i].second));
    }

    if(route.m_indexFiles.size() != 0) {
        for (size_t i = 0; i < route.m_indexFiles.size(); i++){
            m_indexFiles.push_back(route.m_indexFiles[i]);
        }

    }

    m_redirectDir = "";
    m_clearIndexFiles = true;
}

const std::vector<std::string>& ConfigRoute::getDefaultFile() const
{
    return m_indexFiles;
}


const std::string &ConfigRoute::getPath() const
{
   return m_path;
}

const std::string &ConfigRoute::getRoot() const
{
    return m_root;
}

int ConfigRoute::getAllowedMethods() const
{
    return m_allowedMethods;
}

size_t ConfigRoute::getClientMaxBodySize() const
{
    return m_clientMaxBodySize;
}

bool ConfigRoute::getAutoIndex() const
{
    return m_autoindex;
}

const std::vector<std::pair<std::string, std::string> > &ConfigRoute::getCgi() const
{
    return m_cgi;
}

const std::string &ConfigRoute::getRedirectDir() const
{
    return m_redirectDir;
}

void ConfigRoute::setPath(const std::string &path)
{
    m_path = path;
}

void ConfigRoute::setAllowedMethods(int methods)
{
    m_allowedMethods = methods;
}

void ConfigRoute::AddIndexFile(const std::string &newIndexFile)
{
    if(m_clearIndexFiles){
        m_indexFiles.clear();
        m_clearIndexFiles = false;
    }

    if (std::find(m_indexFiles.begin(),m_indexFiles.end(),newIndexFile) == m_indexFiles.end())
        m_indexFiles.push_back(newIndexFile);
    else {
        PRINT("WARNING, index file already exists, skipping this one");
    }

}

void ConfigRoute::setRoot(const std::string &newRoot)
{
    m_root = newRoot;
}

void ConfigRoute::setClientMaxBodySize(size_t size)
{
    m_clientMaxBodySize = size;
}

void ConfigRoute::setAutoindex(bool value)
{
    m_autoindex = value;
}

void ConfigRoute::addCGI(const std::string &key, const std::string &value)
{
    std::vector<std::pair<std::string,std::string> >::iterator it = m_cgi.begin() ;
    for (; it < m_cgi.end(); it++){
        if(it->first == key)
            break;
    }

    if(it == m_cgi.end()){
        PRINT_LOG("CGI key was not found, added new pair: ",key,value);
        m_cgi.push_back(std::pair<std::string, std::string>(key,value));
    }
    else{
        PRINT_WARNING("CGI key already exists, value will be overwritten, old =",it->second,"new = ",value);
        it->second = value;
    }
}

int ConfigRoute::findValidIndexFile(std::string &path) const
{
    std::string fullpath;
    int fileInfo;
    for (size_t i = 0; i < m_indexFiles.size(); i++)
    {
        fullpath = path + m_indexFiles[i];
        fileInfo = ValidatePath(fullpath);
        if(fileInfo == S_IFREG){
            path += m_indexFiles[i];
            return fileInfo;
        }
    }
    PRINT_WARNING("No index valid index file could be found for",path);
    return -1;
}

void ConfigRoute::setRedirectDir(const std::string &dir)
{
    m_redirectDir = dir;
}

std::ostream &operator<<(std::ostream &os, const ConfigRoute &cr)
{
    std::string spaces("     ");

    os << spaces  << "--------Config ROUTE--------" << std::endl;

    os << spaces << "Path = " << cr.getPath() << std::endl;
    os << spaces << "Root dir = " << cr.m_root << std::endl;
    os << spaces << "Index File(s) = ";
    if(cr.m_indexFiles.size() != 0) {
        for (size_t i = 0; i < cr.m_indexFiles.size(); i++)
            os <<  cr.m_indexFiles[i] << " ";
        os << std::endl;
    }
    else
        os << "NONE are setup" << std::endl;
    os << spaces << "Allowed Methods = " << MethodEnumToString(cr.m_allowedMethods) << std::endl;
    os << spaces << "Client Body Buff Size = " << cr.m_clientMaxBodySize << std::endl;
    os << spaces << "Autoindex = " << cr.m_autoindex << std::endl;

    os << spaces << spaces << "----CGI----" << std::endl;

    if(cr.m_cgi.size() != 0) {

        for (size_t i = 0; i < cr.m_cgi.size(); i++)
            os << spaces << spaces << "elem " <<i << ": " <<cr.m_cgi[i].first <<" " << cr.m_cgi[i].second << std::endl;
    }
    else{
         os << spaces << spaces << "no cgi found" << std::endl;
    }

    os << spaces << "Should Redirect? = " ;
    if(cr.m_redirectDir.empty())
        os << "no" << std::endl;
    else{
        os << "yes, to: " << cr.m_redirectDir << std::endl;
    }
    return os;
}
