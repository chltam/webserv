#pragma once

#include <iostream>
#include <map>
#include <vector>
#include "../includes/webserv.hpp"

class ConfigRoute
{
public:
    ConfigRoute();
    ConfigRoute(const std::string& path,const std::string& root, const std::string& defaultFile, int allowedMethods,size_t bodyBufferSize, bool autoIndex, const std::string& redirectDir = "");
    ConfigRoute(const ConfigRoute& route);

    friend std::ostream& operator<< (std::ostream& os, const ConfigRoute& cr);

public: //getters & setters
    const std::string& getPath() const;
    const std::string& getRoot() const;
    const std::vector<std::string>& getDefaultFile() const;
    int getAllowedMethods() const;
    size_t getClientBodyBufferSize() const;
    bool getAutoIndex() const;
    const std::vector<std::pair<std::string, std::string>>& getCgi() const;
    const std::string& getRedirectDir() const;


    void setPath(const std::string& path);
    void setAllowedMethods(int methods);
    void setRoot(const std::string& newRoot);
    void setClientBodyBufferSize(size_t size);
    void setAutoindex(bool value);
    void setRedirectDir(const std::string& dir);

    void AddIndexFile(const std::string& newDefaultFile);
    void addCGI(const std::string& key, const std::string& value);
    
private:
    std::string m_path;
    std::string m_root;
    std::vector<std::string> m_indexFiles;
    int m_allowedMethods;
    size_t m_clientBodyBufferSize;
    bool m_autoindex;
    std::vector<std::pair<std::string, std::string>> m_cgi;

    std::string m_redirectDir;

    bool m_clearIndexFiles;

};
