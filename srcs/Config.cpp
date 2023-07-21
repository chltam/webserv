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


Config::Config(char *filepath):m_brackCount(0)
{
    std::string path = "";
    if(filepath == NULL){
        PRINT_WARNING("Filepath is NULL, using Default.conf");
        path = "Default.conf";
    }
    else
        path = filepath;

    TokenQueue tokens;
    Tokenizer(path,tokens);
    Lexer(tokens);

    // for (int i = 0; i < tokens.size(); i++)
    //     printf( "%-10s : %s\n", TokenToString(tokens[i].second).c_str(), tokens[i].first.c_str());

    Node head;
    Parser(tokens,head);

    printNode(head);

    Executioner(head);
}

Config::~Config()
{

}

void Config::Tokenizer(const std::string &filepath, TokenQueue &tokens)
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
        PRINT_ERROR("Failed to open the file.");
        exit(1);
    }

    // std::cout << fileContents << std::endl;
    size_t prev = 0;
    int flag = 0; //1 = completed word, 2 = quotes, 3 = comment
    char quoteType;
    for (size_t i = 0; i <= fileContents.length(); i++) {
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
            if(fileContents[i] == ';' || fileContents[i] == '{' || fileContents[i] == '}' )
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

void Config::Parser(TokenQueue& tokens, Node& head)
{
    ParseConfig(tokens,head);
    if(m_brackCount != 0) {
        PRINT_ERROR("ERROR, NOT MATCHING BRACKETS");
        PRINTVAR(m_brackCount);
        exit(1);
    }
}

void Config::ParseConfig(TokenQueue& tokens, Node& currNode)
{
    while (tokens.size()){
        if(tokens[0].second == T_CONF_BLOCKEND && m_brackCount > 0){
            tokens.pop_front();
            m_brackCount--;
            return;
        }

        ParseStatement(tokens,currNode);
    }
}

void Config::ParseStatement(TokenQueue& tokens, Node& currNode)
{
    if(tokens[0].second == T_CONF_BLOCK_SERVER && tokens[1].second == T_CONF_BLOCKSTART && currNode.type == NODE_CONFIGURATION){
        currNode.children.emplace_back(Node(NODE_BLOCK_SERVER,tokens[0].first));
        for (int i = 0; i < 2; i++)
            tokens.pop_front();
        m_brackCount++;
        ParseConfig(tokens,currNode.children.back());
    }
    else if(tokens[0].second == T_CONF_BLOCK_LOCATION && tokens[1].second == T_CONF_LITERAL
     && tokens[2].second == T_CONF_BLOCKSTART && currNode.type == NODE_BLOCK_SERVER){
        currNode.children.emplace_back(Node(NODE_BLOCK_LOCATION,tokens[0].first));
        currNode.children.back().values.emplace_back(tokens[1].first);
        m_brackCount++;
        for (int i = 0; i < 3; i++)
            tokens.pop_front();
        ParseConfig(tokens,currNode.children.back());
    }
    else if(tokens[0].second == T_CONF_KEYWORD){
        currNode.children.emplace_back(Node(NODE_DIRECTIVE,tokens[0].first));
        tokens.pop_front();
        ParseDirective(tokens, currNode.children.back());
    }
    else if( tokens[0].second == T_CONF_SEMICOLON
        && (tokens[1].second == T_CONF_KEYWORD || tokens[1].second == T_CONF_BLOCKEND || tokens[1].second == T_CONF_BLOCK_LOCATION)){
            tokens.pop_front();
        }
    else if(tokens[0].second == T_CONF_BLOCKEND && tokens[1].second == T_CONF_KEYWORD){
        return;
    }
    else{
        PRINT_ERROR("PARSE ERROR, IN PARSE STATEMENT!!!!");
        // PRINTVAR(tokens[0].first);
        // PRINTVAR(tokens[1].first);
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
         PRINT_ERROR("PARSE ERROR, IN PARSE DIRECTIVE!!!!");
         PRINTVAR(currNode.name);
         PRINTVAR(tokens[0].first);
         exit(1);
    }
}

void Config::Executioner(Node &head)
{
    //set default root route foreach Server

    for (int i = 0; i < head.children.size(); i++) {
        createConfigServer(head.children[i]);
    }

    for (int i = 0; i < m_servers.size(); i++){
        if(m_servers[i].getPorts().size() != 0)
            return;
    }
    //adding a default Server if no servers have been setup at all
    PRINT_WARNING("WARNING, No server has any ports setup, adding default 0.0.0.0:8080 to first server block");
    m_servers[0].AddServerPort("0.0.0.0","8080");

}

void Config::createConfigServer(Node &serverNode)
{
    m_servers.emplace_back(ConfigServer());
    ConfigServer& lastServer = m_servers.back();
    ConfigRoute* defaultRoute = new ConfigRoute("/","./files","index.html",METH_GET,100000000,false,""); // setting up default file
    for (int i = 0; i < serverNode.children.size(); i++) { //loop over all directive nodes to create a default route
        if(serverNode.children[i].name == "listen")
            AddServerPort(lastServer,serverNode.children[i].values[0]);
        else if (serverNode.children[i].type == NODE_DIRECTIVE){
            updateConfigRoute(*defaultRoute,serverNode.children[i],NODE_BLOCK_SERVER);
        }
        else if(serverNode.children[i].type == NODE_BLOCK_LOCATION) //skip location blocks for the time being
            continue;
        else{
            PRINT_ERROR("ERROR HAPPENED SOMEWHERE, in create config Server ");
            PRINTVAR(serverNode.children[i].name);
            exit(1);
        }
    }
    lastServer.AddConfigRoute(defaultRoute);

    PRINT("-----------CREATING NEW CONFIGROUTES!!!!---------------");
    for (int i = 0; i < serverNode.children.size(); i++) {
        if (serverNode.children[i].type == NODE_BLOCK_LOCATION) {
            ConfigRoute* newRoute = new ConfigRoute(*(lastServer.getRouteFromPath(serverNode.children[i].values[0]))); //might need error checking? what if NULL?
            newRoute->setPath(serverNode.children[i].values[0]);
            for (int j = 0; j < serverNode.children[i].children.size(); j++) {
                updateConfigRoute(*newRoute,serverNode.children[i].children[j],NODE_BLOCK_LOCATION);
            }
            lastServer.AddConfigRoute(newRoute);
        }

    }
    if(lastServer.getPorts().size() == 0)
        PRINT_WARNING("WARNING, no port has been setup for server, behavior might be wrong");
}

void Config::updateConfigRoute(ConfigRoute& route,Node &currNode, NodeType type )
{
    PRINT("UPDATING ROUTE");
    //switch statement with loads of functions to update a route
    // allow_methods index root

    if(currNode.name == "allow_methods"){
        int allowedMethods = METH_NONE;
        for (int i = 0; i < currNode.values.size(); i++) {
           allowedMethods |= StringToMethodEnum(currNode.values[i]);
        }
        route.setAllowedMethods(allowedMethods);
    }
    else if(currNode.name == "index"){
        for (int i = 0; i < currNode.values.size(); i++) {
            route.AddIndexFile(currNode.values[i]); // behavior unclear for overwriting
        }
    }
    else if(currNode.name == "root"){
        route.setRoot(currNode.values[0]);
    }
    else if(currNode.name == "auto_index"){
        if(currNode.values[0] == "on" || currNode.values[0] == "1" )
            route.setAutoindex(true);
        else if(currNode.values[0] == "off" || currNode.values[0] == "0" )
            route.setAutoindex(false);
        else
            PRINT_WARNING("WARNING! autoindex read invalid value, val will not be changed");
    }
    else if(currNode.name == "return"){
        route.setRedirectDir(currNode.values[0]);
    }
    else if(currNode.name == "client_body_buffer_size"){
        std::stringstream sstream(currNode.values[0]);
        size_t result;
        sstream >> result;
        route.setClientBodyBufferSize(result);
    }
    else if(currNode.name == "cgi"){
        route.addCGI(currNode.values[0],currNode.values[1]);
    }
    else{
        PRINT_ERROR("Error, node name was not recognized.");
        exit(1);
    }

}


void Config::AddServerPort(ConfigServer& currServer,const std::string& initialValue)
{

    // currServer.AddServerPort(value, value);
    //more error checking first port needs to be a number, value needs to be split

    size_t colon= initialValue.find(":");
    if(colon == std::string::npos){
        PRINT_ERROR("ERROR, name:port format is incorrect");
        return;
    }
    std::string key = initialValue.substr(0,colon);
    std::string value = initialValue.substr(colon+1);

    PRINTVAR(initialValue);
    PRINTVAR(key);
    PRINTVAR(value);

    if(key == "localhost")
        key = "127.0.0.1";
    if(value.find_first_not_of("0123456789") != std::string::npos){
        PRINT_ERROR("ERROR, port does not contain only digits");
        return;
    }

    if(m_servers.size() == 0){
        currServer.AddServerPort(key, value);
        return;
    }

    for (int i = 0; i < m_servers.size(); i++) {
        const std::vector<std::pair<std::string,std::string>>& ports = m_servers[i].getPorts();
        for (std::vector<std::pair<std::string,std::string>>::const_iterator it = ports.begin(); it != ports.end(); it++) {
            if(it->first == key && it->second == value){
                PRINT_ERROR("this constellation of servername and host already exists,skipping");
                return;
            }
        }
    }
    currServer.AddServerPort(key, value);
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

const ConfigServer* Config::getConfigServerFromRequest(std::string hostPort) const
{
    //error checking might not be neccesary because if we get here we already know its one of our expected ports

    size_t colon= hostPort.find(":");
    if(colon == std::string::npos){
        PRINT_ERROR("ERROR, name:port format is incorrect");
        return NULL;
    }

    std::string key = hostPort.substr(0,colon);
    std::string value = hostPort.substr(colon+1);


    int port = std::atoi(value.c_str()); //silly step because port string has a trailing new line
    value =  std::to_string(port);

    if(key == "localhost")
        key = "127.0.0.1";
    if(value.find_first_not_of("0123456789") != std::string::npos){
        PRINT_ERROR("ERROR, port does not contain only digits");
        return NULL;
    }

    for (int i = 0; i < m_servers.size(); i++) {
        const std::vector<std::pair<std::string,std::string>>& ports = m_servers[i].getPorts();
        for (std::vector<std::pair<std::string,std::string>>::const_iterator it = ports.begin(); it != ports.end(); it++) {
            if(it->first == key && it->second == value){
                PRINT("FOUND SERVER, returning ");
                return &m_servers[i];
            }
        }
    }

    return NULL;
}

std::ostream &operator<<(std::ostream &os, const Config &config)
{
    os << "-------PRINTING CONFIG---------"<< std::endl;
    for (int i = 0; i < config.m_servers.size(); i++) {
        os << "Server Nr: " << i << std::endl;
        os << config.m_servers[i] << std::endl;
    }

    return os;
}

void printNode(const Node &node,int inset)
{
    std::string offset;
    for (int i = 0; i < inset; i++)
        offset += " ";

    std::string type = nodetypeToString(node.type);
    if(node.type == NODE_BLOCK_SERVER)
        printf("%s\x1B[31m%s\033[0m\n",offset.c_str(),type.c_str());
    else if(node.type == NODE_BLOCK_LOCATION)
        printf("%s\x1B[32m%s\033[0m\n",offset.c_str(),type.c_str());
    else
        printf("%s\x1B[36m%s\033[0m\n",offset.c_str(),type.c_str());
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
    type = NODE_CONFIGURATION;
    name = "HEAD";
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
