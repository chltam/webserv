#include "Config.hpp"

Config::Config(char *filepath)
{
    Tokenizer(filepath);
    Lexer();
    Parser();

    // for (std::vector<std::string>::iterator it = m_rawData.begin(); it != m_rawData.end(); it++) {
    //     int i = 0;
    //     for (; i < 11; i++) {
    //         // std::cout << "checking i =" <<i << std::endl;
    //         // std::cout << "keyword len = " << keywords[i].length() << std::endl;
    //         // std::cout << *it <<std::endl;
    //         if((*it).compare(0,keywords[i].length(),keywords[i]) == 0){
    //             break;
    //         }
    //     }

    //     std::cout << i << std::endl;
    //     switch (i)
    //     {
    //     case 0:
    //         std::cout << "found server" << std::endl;
    //         break;

    //     default:
    //         break;
    //     }
    // }

 for (int i = 0; i < m_rawData.size(); i++)
        std::cout << m_rawData[i].second << ": " << m_rawData[i].first << std::endl;
}

void Config::Tokenizer(char* filepath)
{
    std::ifstream input(filepath);
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
                    // std::cerr <<line.size() <<"offset "<< offset <<" added: <" <<line.substr(i,offset) << ">"<< std::endl;
                    // std::cerr <<"I = " <<i << " offset = " << offset << "total = " << i+offset << std::endl;
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

void Config::Parser()
{
    //going through it in 2 passes
        // 1. pass, find all global vars
        // 2. pass, create all routes
    int brackCount = 0;
    for (int i = 0; i < m_rawData.size(); i++){
        std::cout <<"BRACK COUNT = "<< brackCount << std::endl;
        if(brackCount == 0 && m_rawData[i].second == T_SERVER && m_rawData[i + 1].second == T_BRACK_OPN){
            m_servers.push_back(ConfigServer());
            brackCount++;
        }
        else if(m_rawData[i].second == T_LOCATION && m_rawData[i + 2].second == T_BRACK_OPN){
            //skip location blocks for now
            while(m_rawData[i].second != T_BRACK_CLS)
                i++;
        }
        else if(m_rawData[i].second == T_BRACK_CLS)
            brackCount--;
        else if(m_rawData[i].second != T_VALUE){
            //found a key that needs to be sorted out
            int currServerId = m_servers.size() -1;
            std::string str;
            size_t pos;

            switch (m_rawData[i].second)
            {
            case T_LISTEN:
                std::cout << "found listen" << std::endl;
                str = m_rawData[i+1].first;
                pos = str.find(":");
                if (pos != std::string::npos)
                    m_servers[currServerId].m_ports.emplace_back(std::pair<std::string,std::string>(str.substr(0,pos),str.substr(pos+1)));
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
            case T_ALLOW_METH:
                m_servers[currServerId].m_allowedMethods = 0; //reset value to be overwritten
                i++;
                while(m_rawData[i].second == T_VALUE){
                    std::cout << "here: "<<m_rawData[i].second << std::endl;
                    if(m_rawData[i].first == "GET"){
                        std::cout << "found get" << std::endl;
                        std::cout << m_rawData[i].first << std::endl;
                         m_servers[currServerId].m_allowedMethods |= METH_GET;
                    }
                    else if(m_rawData[i].first == "POST"){
                        std::cout << "found POST" << std::endl;
                         m_servers[currServerId].m_allowedMethods |= METH_POST;
                    }
                    else if(m_rawData[i].first == "DELETE")
                         m_servers[currServerId].m_allowedMethods |= METH_DELETE;
                    i++;
                }
                break;
            default:
                break;
            }
        }
    }

    for (int i = 0; i < m_servers.size(); i++)
    {
        std::cout << "Server Nr: " << i << std::endl;
        std::cout << m_servers[i] << std::endl;
    }

}

const std::vector<ConfigServer> &Config::getServers() const
{
    return m_servers;
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
