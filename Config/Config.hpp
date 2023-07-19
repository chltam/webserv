#pragma once

#include <vector>
#include <map>
#include <queue>
#include <algorithm>
#include <iostream>
#include <fstream>
#include "../includes/webserv.hpp"
#include "ConfigServer.hpp"

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
    const std::vector<ConfigServer>& getServers() const;
    friend std::ostream& operator<< (std::ostream& os, const Config& config);
    // const ConfigServer* getConfigServerFromRequest(std::string hostPort) const;
    std::vector<ConfigServer> m_servers;
private:

    //parsing data
    void Tokenizer(const std::string& filepath, TokenQueue& tokens);
    void Lexer(TokenQueue& tokens);
    void Parser(TokenQueue& tokens, Node& head);
    
    void ParseConfig(TokenQueue& tokens, Node& head);
    void ParseStatement(TokenQueue& tokens, Node& currNode);
    void ParseDirective(TokenQueue& tokens, Node& currNode);

    void Executioner( Node& head);
    void updateConfigServer(ConfigServer& server,Node &currNode);
    void updateConfigRoute(ConfigRoute& route,Node &currNode, NodeType type);

    void createConfigServer(Node &serverNode);
    void AddServerPort(ConfigServer& server,const std::string& value);
    void AddServerName(ConfigServer& server,const std::string& value);
    bool ValidateServerPortNamePairs(ConfigServer& server,const std::string& newvalue);
    //utils
    bool isValidChar(char c);
    std::string TokenToString(int tokenVal);

    //rando utils maybe get rid of this
    int m_brackCount;


};

unsigned int calcAllowedValues(const std::string& key);
void printNode(const Node& node,int inset = 0);
std::string nodetypeToString(int type);