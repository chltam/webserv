#include "../includes/Config.hpp"

Config::Config(char *filepath):m_defaultConfig("Default.conf")
{
    Tokenizer(filepath);
    Lexer();
    Parser();


}

void Config::Tokenizer(char* filepath)
{
    //do more file error checking
    std::string setup;
    if(filepath == NULL)
        setup = m_defaultConfig;
    else
        setup = filepath;

    std::ifstream input(setup);
    std::string line;
    if(input.is_open()){
        while(std::getline(input,line)){
            // std::cout <<"<"<< line <<">" << std::endl;
            for (int i = 0;i < line.length(); i++) {
                int offset = 0;
                while(isValidChar(line[i+offset]) && i + offset < line.length())
                    offset++;
                if(line[i] == '#')
                    break;
                if(offset > 0){
                    m_rawData.push_back(std::pair<std::string,int>(line.substr(i,offset), -1));
                }
                i+= offset;
                if(line[i] == ';')
                    m_rawData.push_back(std::pair<std::string,int>(";", T_COMMA));
            }



        }

    }
}

void Config::Lexer()
{
    //note this array should match the enum for it to work properly
     std::string keywords[] = {"randomentryforzero","server_name","listen","server",
    "client_body_buffer_size","root","index","allow_methods","location","cgi","auto_index","return","{","}",";"};
    int keywordLen =sizeof(keywords)/sizeof(std::string);
    for (std::vector<std::pair<std::string,int>>::iterator it = m_rawData.begin(); it != m_rawData.end(); it++) {
        int i = 0;
        for (; i < keywordLen; i++) {
            if(keywords[i].length() == (*it).first.length() && (*it).first.compare(0,keywords[i].length(),keywords[i]) == 0){
                (*it).second = i;
                break;
            }
        }
        if(i == keywordLen)
            (*it).second = T_VALUE;
    }
}

// taken out so i can safe routes
//        else if(m_rawData[i].second == T_LOCATION && m_rawData[i + 2].second == T_BRACK_OPN){
//            //skip location blocks for now
//            while(m_rawData[i].second != T_BRACK_CLS)
//                i++;
//        }

void Config::Parser()
{
    int brackCount = 0;
    for (int i = 0; i < m_rawData.size(); i++){
        if(brackCount == 0 && m_rawData[i].second == T_SERVER && m_rawData[i + 1].second == T_BRACK_OPN){
            m_servers.push_back(ConfigServer());
            brackCount++;
        }
        else if(m_rawData[i].second == T_BRACK_CLS){
            //completed looking through server, can add the root directory
            int currServerId = m_servers.size() -1;
            m_servers[currServerId].m_routes.emplace("/",ConfigRoute(m_servers[currServerId]));
            brackCount--;
        }
        else if(m_rawData[i].second != T_VALUE){
            //found a key that needs to be sorted out
            int currServerId = m_servers.size() -1;
            std::string str;
            size_t pos;

            std::string key;

            bool foundCGI = false;
            switch (m_rawData[i].second)
            {
            case T_LISTEN:
                str = m_rawData[i+1].first;
                pos = str.find(":");
                if (pos != std::string::npos)
                    m_servers[currServerId].m_ports.emplace_back(std::pair<std::string,int>(str.substr(0,pos),std::atoi(str.substr(pos+1).c_str())));
                else
                    std::cerr << "ERROR WHEN PARSING LISTEN" << std::endl;
                break;
            case T_SERVER_NAME:
                m_servers[currServerId].m_serverName = m_rawData[i+1].first;
                break;
            case T_BODY_BUFF_SIZE:
                m_servers[currServerId].m_clientBodyBufferSize = std::atoi(m_rawData[i+1].first.c_str());
                break;
            case T_ROOT:
                m_servers[currServerId].m_root = m_rawData[i+1].first;
                break;
            case T_INDEX:
                m_servers[currServerId].m_defaultFile = m_rawData[i+1].first;
                break;
            case T_AUTOINDEX:
                if(m_rawData[i+1].first == "on")
                    m_servers[currServerId].m_autoindex = true;
                else
                    m_servers[currServerId].m_autoindex = false;
                break;
            case T_CGI:
                    m_servers[currServerId].m_cgi.emplace_back(m_rawData[i+1].first,m_rawData[i+2].first);
                break;
            case T_ALLOW_METH:
                m_servers[currServerId].m_allowedMethods = 0; //reset value to be overwritten
                i++;
                while(m_rawData[i].second == T_VALUE){
                    if(m_rawData[i].first == "NONE"){
                         m_servers[currServerId].m_allowedMethods = METH_NONE;
                    }
                    if(m_rawData[i].first == "GET"){
                         m_servers[currServerId].m_allowedMethods |= METH_GET;
                    }
                    else if(m_rawData[i].first == "POST"){
                         m_servers[currServerId].m_allowedMethods |= METH_POST;
                    }
                    else if(m_rawData[i].first == "DELETE")
                         m_servers[currServerId].m_allowedMethods |= METH_DELETE;
                    i++;
                }
                break;
            case T_LOCATION:
                //currently assuming that the locations come after the global configuration
                key = m_rawData[i+1].first;
                m_servers[currServerId].m_routes.emplace(m_rawData[i+1].first,ConfigRoute(m_servers[currServerId]));
                i+=3; // move it to the next key
                while(m_rawData[i].second != T_BRACK_CLS){
                    if(m_rawData[i].second == T_INDEX){
                        m_servers[currServerId].m_routes[key].m_defaultFile = m_rawData[i + 1].first;
                    }
                    else if(m_rawData[i].second == T_ROOT){
                        m_servers[currServerId].m_routes[key].m_root = m_rawData[i + 1].first;
                    }
                    else if(m_rawData[i].second == T_AUTOINDEX){
                        if(m_rawData[i+1].first == "on")
                            m_servers[currServerId].m_routes[key].m_autoindex = true;
                        else
                            m_servers[currServerId].m_routes[key].m_autoindex = false;
                    }
                    else if(m_rawData[i].second == T_RETURN){
                        m_servers[currServerId].m_routes[key].m_shouldRedirect = true;
                        m_servers[currServerId].m_routes[key].m_redirectDir = m_rawData[i + 1].first;
                    }
                    else if(m_rawData[i].second == T_BODY_BUFF_SIZE){
                        m_servers[currServerId].m_routes[key].m_clientBodyBufferSize = std::atoi(m_rawData[i+1].first.c_str());
                    }
                    else if(m_rawData[i].second == T_ALLOW_METH){
                        m_servers[currServerId].m_routes[key].m_allowedMethods = 0;
                        i++;
                        while(m_rawData[i].second == T_VALUE){
                            if(m_rawData[i].first == "NONE"){
                                m_servers[currServerId].m_routes[key].m_allowedMethods = METH_NONE;
                            }
                            else if(m_rawData[i].first == "GET"){
                                m_servers[currServerId].m_routes[key].m_allowedMethods |= METH_GET;
                            }
                            else if(m_rawData[i].first == "POST"){
                                m_servers[currServerId].m_routes[key].m_allowedMethods |= METH_POST;
                            }
                            else if(m_rawData[i].first == "DELETE")
                                m_servers[currServerId].m_routes[key].m_allowedMethods |= METH_DELETE;
                            i++;
                        }
                    }
                    else if(m_rawData[i].second == T_CGI){
                        foundCGI = false;
                        for (int j = 0; j < m_servers[currServerId].m_routes[key].m_cgi.size(); j++)
                        {
                            if(m_servers[currServerId].m_routes[key].m_cgi[j].first == m_rawData[i + 1].first){
                                foundCGI = true;
                                m_servers[currServerId].m_routes[key].m_cgi[j].second = m_rawData[i + 2].first;
                            }
                        }
                        if(foundCGI == false)
                            m_servers[currServerId].m_routes[key].m_cgi.emplace_back(std::pair<std::string,std::string>(m_rawData[i + 1].first,m_rawData[i + 2].first));
                    }
                    i++;
                }
                break;
            default:
                break;
            }
        }
    }
}



const std::vector<ConfigServer> &Config::getServers() const
{
    return m_servers;
}

void Config::printServers()
{
    for (int i = 0; i < m_servers.size(); i++) {
        std::cout << "Server Nr: " << i << std::endl;
        std::cout << m_servers[i] << std::endl;
    }
}

bool Config::isValidChar(char c)
{
    char forbidded[] = " \n\r\t;\0";
    for (int i = 0; i < 5; i++) {
        if(forbidded[i] == c)
            return false;
    }

    return true;
}

const ConfigServer* Config::getConfigServerFromRequest(std::string hostPort) const
{
    //error checking might not be neccesary because if we get here we already know its one of our expected ports

    int port(std::atoi(hostPort.substr(hostPort.find(":")+1).c_str()));
    for (int i = 0; i < m_servers.size(); i++){
        for (int j = 0; j < m_servers[i].m_ports.size(); j++) {

            if(m_servers[i].m_ports[j].second == port){
                std::cerr << "trying to return a server" << std::endl;
                return &m_servers[i];
            }
        }

    }
    std::cerr << "returning NULL" << std::endl;

    return NULL;
}