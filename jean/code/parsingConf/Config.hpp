#pragma once

#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include "ConfigServer.hpp"


enum Tokens{
    T_INVALID = -1,
    T_SERVER_NAME = 1,
    T_LISTEN,
    T_SERVER,
    T_BODY_BUFF_SIZE,
    T_ROOT,
    T_INDEX,
    T_ALLOW_METH,
    T_LOCATION,
    T_CGI,
    T_AUTOINDEX,
    T_RETURN,
    T_BRACK_OPN,
    T_BRACK_CLS,
    T_COMMA,
    T_VALUE
};


class Config
{
public:
    Config(char* filepath);


public: //getters
    const std::vector<ConfigServer>& getServers() const;


private:
    std::vector<ConfigServer> m_servers;


    //parsing data
    void Tokenizer(char* filepath);
    void Lexer();
    void Parser();
    bool isValidChar(char c);
    std::vector<std::pair<std::string,int>> m_rawData;

};
