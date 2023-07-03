#include "RequestParser.hpp"

RequestParser::RequestParser( std::string buffer, int socket ) {

    _buffer = buffer;
    _socket = socket;
};

void RequestParser::tokenizeRequest( void ) {

    // split whole request into lines 
    std::istringstream line_iss( _buffer ); 
    std::vector<std::string> lines;
    std::string tmp_line;
    std::stringstream buffer;

    while ( std::getline( line_iss, tmp_line, '\n' )) {
        lines.push_back( tmp_line );
    }

    std::vector<std::string>::const_iterator it = lines.begin();
    std::istringstream token_iss( *it );
    it++;
    std::vector<std::string> tokens;
    std::string tmp_token;    

    // handle first request line
    while ( std::getline( token_iss, tmp_token, ' ' )) {
        tokens.push_back( tmp_token );
    }
    _reqPairs.push_back(std::make_pair("request type", tokens[0]));
    _reqPairs.push_back(std::make_pair("path", tokens[1]));

    // for (std::vector<std::pair<std::string, std::string> >::const_iterator it = _reqPairs.begin(); it != _reqPairs.end(); ++it) {
    //     std::cout << "Key: " << it->first << ", Value: " << it->second << std::endl;
    // }

    while ( it != lines.end() ) {
        std::istringstream token_iss( *it );
        while ( std::getline( token_iss, tmp_token, ' ' )) {
            tokens.push_back( tmp_token );
        }
        for (std::vector<std::string>::const_iterator it = tokens.begin(); it != tokens.end(); ++it) {
            std::cout << "TOKEN: " << *it << std::endl;
        }
        _reqPairs.push_back(std::make_pair( tokens[0], tokens[1]));
        it++;
    }

    // for (std::vector<std::pair<std::string, std::string> >::const_iterator it = _reqPairs.begin(); it != _reqPairs.end(); ++it) {
    //     std::cout << "Key: " << it->first << ", Value: " << it->second << std::endl;
    // }

 /*    if (tokens[0] == "GET") {

        std::cout << "GET received" << std::endl;

        std::cout << tokens[1] << std::endl;
        std::ifstream file("." + tokens[1]);

        if (file.is_open()) { 
            
            buffer << file.rdbuf();
        } else {
            std::cout << "Unable to open file\n";
        }
        file.close();

        // build http response
        std::string buffStr = buffer.str();
        std::cout << buffStr << std::endl;
        std::string header("HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: 1234\nDate: Thu, 01 Jul 2023 12:34:56 GMT\nServer: Apache\n\n");
        std::string full(header + buffStr);
        // write(m_data[index].m_newSocket,l,sizeof(l));
        write(_socket, (full.c_str()) ,full.length());
    }
    else if (tokens[0] == "POST") {


        std::cout << "POST received" << std::endl;
    } */
}
