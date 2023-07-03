#include "RequestParser.hpp"

RequestParser::RequestParser( std::string buffer ) {

    _buffer = buffer;
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

    // get key-value pairs from lines
    std::vector<std::string>::const_iterator it = lines.begin();
    std::istringstream token_iss( *it );
    it++;
    std::vector<std::string> tokens;
    std::string tmp_token;    

    // handle first request line
    while ( std::getline( token_iss, tmp_token, ' ' )) {
        tokens.push_back( tmp_token );
    }
    _headerPairs.push_back(std::make_pair("request type", tokens[0]));
    _headerPairs.push_back(std::make_pair("path", tokens[1]));
    tokens.clear();

    // handle remaining lines
    while ( it != lines.end() ) {

        // after the header, save the body
        if (it->empty()) {
            ++it;  // skip the empty line
            while (it != lines.end()) {
                _body += *it + '\n';
                ++it;
            }
            break; // we are done
        }
        // save the header info
        size_t split_position = it->find(": ");
        if (split_position != std::string::npos) {
            std::string key = it->substr(0, split_position);
            std::string value = it->substr(split_position + 2); // +2 to skip over ': '
            _headerPairs.push_back(std::make_pair(key, value));
        }
        tokens.clear();
        it++;
    }

    this->printHeaderPairs();
    this->printBody();

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

void RequestParser::printHeaderPairs( void ) {

    for (std::vector<std::pair<std::string, std::string> >::const_iterator it = _headerPairs.begin(); it != _headerPairs.end(); ++it) {
        std::cout << "Key: " << it->first << ", Value: " << it->second << std::endl;
    }
}

void RequestParser::printBody( void ) {

    std::cout << "BODY: " << this->_body << std::endl;
}

std::vector<std::pair<std::string, std::string>> RequestParser::getHeaderPairs( void ) {

    return ( this->_headerPairs );
};

std::string RequestParser::getBody( void ) {

    return ( this->_body );
};
