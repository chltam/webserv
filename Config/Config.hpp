#pragma once

#include <vector>
#include <map>
#include <queue>
#include <algorithm>
#include <iostream>
#include <fstream>
#include "../includes/webserv.hpp"
// #include "ConfigServer.hpp"

enum NodeType
{
    NODE_CONFIGURATION,
    NODE_BLOCK_SERVER,
    NODE_BLOCK_LOCATION,
    NODE_DIRECTIVE
};

struct Node{

    Node();
    Node(NodeType type,std::string name);

    NodeType type;
    std::string name;
    unsigned int totalAllowedValues;
    bool requireSpecificArgs;
    std::vector<std::string> values;
    std::vector<Node> children;

};

typedef std::deque<std::pair<std::string, int>> TokenQueue;

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
    void Tokenizer(const std::string& filepath, TokenQueue& tokens);
    void Lexer(TokenQueue& tokens);
    void Parser(TokenQueue& tokens);
    void ParseConfig(TokenQueue& tokens, Node& head);


    void ParseStatement(TokenQueue& tokens, Node& currNode);
    void ParseDirective(TokenQueue& tokens, Node& currNode);
    void ParseBlock(TokenQueue& tokens, Node& currNode);

    bool isValidChar(char c);
    std::string TokenToString(int tokenVal);

    //rando utils
   
};

unsigned int calcAllowedValues(const std::string& key);
void printNode(const Node& node,int inset = 0);
std::string nodetypeToString(int type);