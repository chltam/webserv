#include "Config.hpp"

Config::Config(char *filepath)
{
    std::ifstream input(filepath);
    std::string line;

    while(getline(input,line)){
        int i = 0;
        while(!isValidChar(line[i]))
            i++;
        if(i < line.length() && line[i] != '#')
            m_rawData.emplace_back(line.substr(i,line.length() - i));
    }

    for (int i = 0; i < m_rawData.size(); i++)
        std::cout << m_rawData[i] << std::endl;

    std::string keywords[] = {"server","listen","server_name",
    "client_body_buffer_size","root","index","allow_methods","cgi","return","{","}"};

    for (std::vector<std::string>::iterator it = m_rawData.begin(); it != m_rawData.end(); it++) {
        int i = 0;
        for (; i < 11; i++) {
            // std::cout << "checking i =" <<i << std::endl;
            // std::cout << "keyword len = " << keywords[i].length() << std::endl;
            // std::cout << *it <<std::endl;
            if((*it).compare(0,keywords[i].length(),keywords[i]) == 0){
                break;
            }
        }

        std::cout << i << std::endl;
        switch (i)
        {
        case 0:
            std::cout << "found server" << std::endl;
            break;

        default:
            break;
        }
    }


}

const std::vector<ConfigServer> &Config::getServers() const
{
    return m_servers;
}

bool Config::isValidChar(char c)
{
    char forbidded[] = " \n\r\t\0";
    for (int i = 0; i < 5; i++) {
        if(forbidded[i] == c)
            return false;
    }

    return true;
}
