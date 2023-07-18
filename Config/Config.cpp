#include "Config.hpp"

// <nginx-conf> ::= <statement> | <statement> <nginx-conf>
// <statement> ::= <directive> ";" | <block>
// <directive> ::= <identifier> <args>
// <block> ::= <identifier> "{" <nginx-conf> "}"
// <args> ::= <arg> | <arg> <args>
// <arg> ::= <string> | <number> | <identifier>

// <identifier> ::= [a-zA-Z_][a-zA-Z0-9_]*
// <string> ::= '"' <characters> '"' | "'" <characters> "'"
// <number> ::= [0-9]+

// <characters> ::= <character> | <character> <characters>
// <character> ::= any printable ASCII character except '"' or "'"


enum ConfigToken
{
    T_CONF_KEYWORD = 1,
    T_CONF_IDENTIFIER,
    T_CONF_LITERAL,
    T_CONF_SEMICOLON,
    T_CONF_BLOCK_SERVER,
    T_CONF_BLOCK_LOCATION,
    T_CONF_BLOCKSTART,
    T_CONF_BLOCKEND,
    
};


Config::Config(char *filepath)
{
    std::string path;
    if(filepath == NULL){
        PRINT("Filepath is NULL, using Default.conf");
        path = "Default.conf";
    }
    else
        path = filepath;

    TokenQueue tokens;
    Tokenizer(path,tokens);
    Lexer(tokens);
    // for (int i = 0; i < tokens.size(); i++)
    //     printf( "%-10s : %s\n", TokenToString(tokens[i].second).c_str(), tokens[i].first.c_str());
    
    
    Parser(tokens);
}

void Config::Tokenizer(const std::string& filepath, TokenQueue& tokens)
{
    PRINT("STARTED TOKENIZING");

    std::ifstream inputFile(filepath);
    std::string fileContents;
    if (inputFile.is_open()) {
        std::stringstream buffer;
        buffer << inputFile.rdbuf();
        fileContents = buffer.str();
        inputFile.close();
    } else {
        std::cout << "Failed to open the file." << std::endl;
        exit(1);
    }

    // std::cout << fileContents << std::endl;
    size_t prev = 0;
    int flag = 0; //1 = completed word, 2 = quotes, 3 = comment
    char quoteType;
    for (size_t i = 0; i < fileContents.length(); i++) {
        if ((fileContents[i] == ';' || fileContents[i] == '{' || fileContents[i] == '}') && flag == 0) {
            tokens.push_back(std::pair<std::string,int>(std::string{fileContents[i]},-1));
            prev = i;
        }
        else if(fileContents[i] == '#' && flag == 0){ // found comments
            flag = 3;
            prev = i;
        }
        else if((fileContents[i] == '\n' || fileContents[i] == '\0') && flag == 3){
            flag = 0;
            prev = i;
        }
        else if((fileContents[i] == '\'' || fileContents[i] == '"') && flag == 0) { //quote start
            quoteType = fileContents[i];
            flag = 2;
            prev = i;
        }
        else if(quoteType == fileContents[i] && flag == 2) { // quote end
            tokens.push_back(std::pair<std::string,int>(fileContents.substr(prev,i - prev + 1),-1));
            flag = 0;
            prev = i;
        }
        else if(isValidChar(fileContents[i]) && flag == 0) {
            flag = 1;
            prev = i;
        }
        else if(!isValidChar(fileContents[i]) && flag == 1) {
            tokens.push_back(std::pair<std::string,int>(fileContents.substr(prev,i - prev),-1));
            if(fileContents[i] == ';' || fileContents[i] == '{' || fileContents[i] == '}')
                tokens.push_back(std::pair<std::string,int>(std::string{fileContents[i]},-1));
            flag = 0;
            prev = i;
        }
    }
    return ;
}

void Config::Lexer(TokenQueue& tokens)
{
    std::vector<std::string> keywords = {"listen","server_name","auto_index",
            "client_body_buffer_size","root","index","allow_methods","cgi","return"};

    for (size_t i = 0; i < tokens.size(); i++) {
        if(tokens[i].first == ";")
            tokens[i].second = T_CONF_SEMICOLON;
        else if(tokens[i].first == "{")
            tokens[i].second = T_CONF_BLOCKSTART;
        else if(tokens[i].first == "}")
            tokens[i].second = T_CONF_BLOCKEND;
        else if(tokens[i].first[0] == '\'' || tokens[i].first[0] == '"') {
            tokens[i].second = T_CONF_LITERAL;
            tokens[i].first = tokens[i].first.substr(1,tokens[i].first.length() -2);
        }
        else if(tokens[i].first == "server")
            tokens[i].second = T_CONF_BLOCK_SERVER;
        else if(tokens[i].first == "location")
            tokens[i].second = T_CONF_BLOCK_LOCATION;
        else if(std::find(keywords.begin(),keywords.end(),tokens[i].first) != keywords.end())
            tokens[i].second = T_CONF_KEYWORD;
        else
            tokens[i].second = T_CONF_LITERAL;
    }

}

void Config::Parser(TokenQueue& tokens)
{
    Node head;
    head.type = NODE_CONFIGURATION;
    head.name = "HEAD";
    ParseConfig(tokens,head);
    for (int i = 0; i < tokens.size(); i++)
        printf( "%-10s : %s\n", TokenToString(tokens[i].second).c_str(), tokens[i].first.c_str());
    printNode(head);
}

void Config::ParseConfig(TokenQueue& tokens, Node& currNode)
{
    while (tokens.size()){
        if(tokens[0].second == T_CONF_BLOCKEND){
            tokens.pop_front();
            return;
        }
        ParseStatement(tokens,currNode);
    }
}

void Config::ParseStatement(TokenQueue& tokens, Node& currNode)
{
    if(tokens[0].second == T_CONF_BLOCK_SERVER && tokens[1].second == T_CONF_BLOCKSTART && currNode.type == NODE_CONFIGURATION){
        currNode.children.emplace_back(Node(NODE_BLOCK_SERVER,tokens[0].first));
        tokens.pop_front(); 
        tokens.pop_front(); 
        ParseConfig(tokens,currNode.children.back());
    }
    else if(tokens[0].second == T_CONF_BLOCK_LOCATION && tokens[1].second == T_CONF_LITERAL
     && tokens[2].second == T_CONF_BLOCKSTART && currNode.type == NODE_BLOCK_SERVER){
        ParseStatement(tokens,currNode.children.back());
    }
    else if(tokens[0].second == T_CONF_KEYWORD){
        PRINT("IN HERE");
        currNode.children.emplace_back(Node(NODE_DIRECTIVE,tokens[0].first));
        tokens.pop_front();
        ParseDirective(tokens, currNode.children.back());
    }
    else if( tokens[0].second == T_CONF_SEMICOLON)
        tokens.pop_front();
    else if(tokens[0].second == T_CONF_BLOCKEND){
        return;
    }
    else{
        PRINT("PARSE ERROR, IN PARSE STATEMENT!!!!");
        PRINTVAR(tokens[0].first);
        exit(1);
    }

    return;    
}

void Config::ParseDirective(TokenQueue& tokens, Node& currNode)
{
    if(tokens[0].second == T_CONF_LITERAL && tokens[1].second == T_CONF_LITERAL && currNode.totalAllowedValues > 0){
        currNode.values.emplace_back(tokens[0].first);
        tokens.pop_front();
        currNode.totalAllowedValues--;
        ParseDirective(tokens,currNode);
    }
    else if(tokens[0].second == T_CONF_LITERAL && tokens[1].second == T_CONF_SEMICOLON
         && currNode.totalAllowedValues == 1 && currNode.requireSpecificArgs){
        currNode.values.emplace_back(tokens[0].first);
        tokens.pop_front();
        currNode.totalAllowedValues--;
        return;
    }
    else if(tokens[0].second == T_CONF_LITERAL && tokens[1].second == T_CONF_SEMICOLON && !currNode.requireSpecificArgs){
            currNode.values.emplace_back(tokens[0].first);
            tokens.pop_front();
            currNode.totalAllowedValues--;
            return;
         }
    else{
         PRINT("PARSE ERROR, IN PARSE DIRECTIVE!!!!");
         PRINTVAR(currNode.name);
         PRINTVAR(tokens[0].first);
         exit(1);
    }
}

void Config::ParseBlock(TokenQueue& tokens, Node& currNode)
{

}



bool Config::isValidChar(char c)
{
    char forbidded[] = " \n\r\t;{}\0";
    for (int i = 0; i < sizeof(forbidded); i++) {
        if(forbidded[i] == c)
            return false;
    }

    return true;
}


// const std::vector<ConfigServer> &Config::getServers() const
// {
//     return m_servers;
// }

// const ConfigServer* Config::getConfigServerFromRequest(std::string hostPort) const
// {
//     //error checking might not be neccesary because if we get here we already know its one of our expected ports

//     int port(std::atoi(hostPort.substr(hostPort.find(":")+1).c_str()));
//     for (int i = 0; i < m_servers.size(); i++){
//         for (int j = 0; j < m_servers[i].m_ports.size(); j++) {

//             if(m_servers[i].m_ports[j].second == port){
//                 std::cerr << "trying to return a server" << std::endl;
//                 return &m_servers[i];
//             }
//         }

//     }
//     std::cerr << "returning NULL" << std::endl;

//     return NULL;
// }

std::ostream &operator<<(std::ostream &os, const Config &config)
{
    // for (int i = 0; i < config.m_servers.size(); i++) {
    //     os << "Server Nr: " << i << std::endl;
    //     os << config.m_servers[i] << std::endl;
    // }
    os << "-------PRINTING---------"<< std::endl;

    return os;
}

void printNode(const Node &node,int inset)
{
    std::string offset;
    for (int i = 0; i < inset; i++)
        offset += " ";
    
    std::cout << offset << "Type: " << nodetypeToString(node.type) << std::endl;
    std::cout << offset << "Directive: " << node.name << std::endl;
    std::cout << offset << "tot Values: " << node.totalAllowedValues << std::endl;
    std::cout << offset << "bool: " << node.requireSpecificArgs << std::endl;

    for (int i = 0; i < node.values.size(); i++)
        std::cout << offset << node.values[i] << std::endl;
    std::cout << offset << "-----Node Children-----" << std::endl;
    for (int i = 0; i < node.children.size(); i++){
        std::cout << offset << "Child: " << i << std::endl;
        printNode(node.children[i],inset+5);
    }
    
}

std::string Config::TokenToString(int tokenVal)
{
    switch (tokenVal)
    {
    case T_CONF_KEYWORD:
        return "KEYWORD";
    case T_CONF_IDENTIFIER:
        return "IDENTIFIER";
    case T_CONF_LITERAL:
        return "LITERAL";
    case T_CONF_SEMICOLON:
        return "SEMICOLON";
    case T_CONF_BLOCKSTART:
        return "BLOCKSTART";
    case T_CONF_BLOCKEND:
        return "BLOCKEND";
    case T_CONF_BLOCK_SERVER:
        return "SERVER";
    case T_CONF_BLOCK_LOCATION:
        return "LOCATION";
    }
    return "INVALID TOKEN";
}

unsigned int calcAllowedValues(const std::string &key)
{
    std::vector<std::string> infititeKeywords = {"index","allow_methods","server_name"};
    if(std::find(infititeKeywords.begin(),infititeKeywords.end(),key) != infititeKeywords.end())
        return -1; // will be unsigned int max
    else if(key == "cgi")
        return 2;
    return 1;
}

Node::Node()
{

}

Node::Node(NodeType type, std::string name)
{
    this->type = type;
    this->name = name;
    this->totalAllowedValues = calcAllowedValues(name);
    unsigned int val = -1;
    if(totalAllowedValues != val)
        this->requireSpecificArgs = true;
    else
         this->requireSpecificArgs = false;
}

std::string nodetypeToString(int type)
{
    switch (type)
    {
    case NODE_CONFIGURATION:
        return "NODE_CONFIGURATION";
    case NODE_BLOCK_SERVER:
        return "NODE_BLOCK_SERVER";
    case NODE_BLOCK_LOCATION:
        return "NODE_BLOCK_LOCATION";
    case NODE_DIRECTIVE:
        return "NODE_DIRECTIVE";
    }
    return "ERROR";
}