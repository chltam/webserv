#pragma once

#include <vector>
#include <iostream>
#include <fstream>
#include "ConfigServer.hpp"

class Config
{
public:
    Config(char* filepath);


public: //getters
    const std::vector<ConfigServer>& getServers() const;


private:
    std::vector<ConfigServer> m_servers;


    //parsing data
    bool isValidChar(char c);
    std::vector<std::string> m_rawData;

};
