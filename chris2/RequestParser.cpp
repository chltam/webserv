#include "RequestParser.hpp"

RequestParser::RequestParser( std::string buffer, int socket ) {

    _buffer = buffer;
    _socket = socket;
};

void RequestParser::tokenizeRequest( void ) {

    // tokenize the request line 
    std::istringstream iss( _buffer ); 
    std::vector<std::string> tokens;
    std::string temp;
    std::stringstream buffer;

    while (std::getline(iss, temp, ' ')) {
        tokens.push_back(temp);
    }

    if (tokens[0] == "GET") {

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
    }
}
