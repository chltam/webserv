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

    std::vector<std::pair<std::string, int>> tokens;
    Tokenizer(path,tokens);
    Lexer(tokens);

    for (int i = 0; i < tokens.size(); i++)
        printf( "%-10s : %s\n", TokenToString(tokens[i].second).c_str(), tokens[i].first.c_str());
}

void Config::Tokenizer(const std::string& filepath, std::vector<std::pair<std::string, int>>& tokens)
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
            tokens.emplace_back(std::pair<std::string,int>(std::string{fileContents[i]},-1));
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
            tokens.emplace_back(std::pair<std::string,int>(fileContents.substr(prev,i - prev + 1),-1));
            flag = 0;
            prev = i;
        }
        else if(isValidChar(fileContents[i]) && flag == 0) {
            flag = 1;
            prev = i;
        }
        else if(!isValidChar(fileContents[i]) && flag == 1) {
            tokens.emplace_back(std::pair<std::string,int>(fileContents.substr(prev,i - prev),-1));
            if(fileContents[i] == ';' || fileContents[i] == '{' || fileContents[i] == '}')
                tokens.emplace_back(std::pair<std::string,int>(std::string{fileContents[i]},-1));
            flag = 0;
            prev = i;
        }
    }
    return ;
}

void Config::Lexer(std::vector<std::pair<std::string, int>>& tokens)
{
    std::vector<std::string> keywords = {"server","location","listen","server_name","auto_index",
            "client_body_buffer_size","root","index","allow_methods","cgi"};

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
        else if(std::find(keywords.begin(),keywords.end(),tokens[i].first) != keywords.end())
            tokens[i].second = T_CONF_KEYWORD;
        else
            tokens[i].second = T_CONF_LITERAL;
    }

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
    os << "PRINTING"<< std::endl;

    return os;
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
    }
    return "INVALID TOKEN";
}