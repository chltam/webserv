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
    ~Config();

public: //getters
    friend std::ostream& operator<< (std::ostream& os, const Config& config);
    const ConfigServer* getConfigServerFromRequest(std::string hostPort) const;
    std::vector<ConfigServer> m_servers;
    const std::map<std::string, std::string>& getTypes() const;

private:

    //parsing data
    void Tokenizer(const std::string& filepath, TokenQueue& tokens);
    void Lexer(TokenQueue& tokens);
    void Parser(TokenQueue& tokens, Node& head);

    void ParseConfig(TokenQueue& tokens, Node& head);
    void ParseStatement(TokenQueue& tokens, Node& currNode);
    void ParseDirective(TokenQueue& tokens, Node& currNode);

    void Executioner( Node& head);
    void updateConfigRoute(ConfigRoute& route,Node &currNode, NodeType type);

    void createConfigServer(Node &serverNode);
    void AddServerPort(ConfigServer& server,const std::string& value);
    void extractMimesFromFile();

    //utils
    bool isValidChar(char c);
    std::string TokenToString(int tokenVal);

    //rando utils maybe get rid of this
    int m_brackCount;
    std::map<std::string, std::string> m_types;


};

unsigned int calcAllowedValues(const std::string& key);
void printNode(const Node& node,int inset = 0);
std::string nodetypeToString(int type);
