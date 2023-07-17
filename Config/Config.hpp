#pragma once

#include <vector>
#include <map>
#include <algorithm>
#include <iostream>
#include <fstream>
#include "../includes/webserv.hpp"
// #include "ConfigServer.hpp"

class Config
{
public:
    Config(char* filepath);


public: //getters
    // const std::vector<ConfigServer>& getServers() const;
    void printServers();
    friend std::ostream& operator<< (std::ostream& os, const Config& config);
    // const ConfigServer* getConfigServerFromRequest(std::string hostPort) const;
private:
    // std::vector<ConfigServer> m_servers;

    //parsing data
    void Tokenizer(const std::string& filepath, std::vector<std::pair<std::string, int>>& tokens);
    void Lexer(std::vector<std::pair<std::string, int>>& tokens);

    bool isValidChar(char c);
    std::string TokenToString(int tokenVal);
};
