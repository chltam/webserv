#include "ResponseBuilder.hpp"

ResponseBuilder::ResponseBuilder( int socket, std::vector<std::pair<std::string,
    std::string>> headerPairs, std::string body ) {

    _socket = socket;
    _reqHeaderPairs = headerPairs;
    _reqBody = body;
};

void ResponseBuilder::buildResponse( void ) {

    buildHeader();
    buildBody();

    // CONTENT TYPE is hard coded because Firefox sends too much info (more parsing needed)
    _respHeader = "HTTP/1.1 " + _status + _statusMsg + /* "\r\nConnection: keep-alive" + */ "\r\nContent-Type: text/html"\
        + "\r\nContent-Length: " + _contLen + "\r\nDate: " + _dateTime + "\r\nServer: " + _serverName + "\r\n\r\n";
}

void ResponseBuilder::buildHeader( void ) {

    std::string header;
    
    for (std::vector<std::pair<std::string, std::string> >::const_iterator it = _reqHeaderPairs.begin();
        it != _reqHeaderPairs.end(); ++it) {
        
        if ( it->first == "request type" ) {
            _reqType = it->second;
        }
        else if ( it->first == "path" ) {
            _path = it->second;
        }
        else if ( it->first == "Host" ) {
            _serverName = it->second;
        }
        else if ( it->first == "Accept" ) {
            _contType = it->second;
        }
    };
    saveDateTime();
    determineStatus();
}

void ResponseBuilder::determineStatus( void ) {

    // how is it determined?
    if ( _reqType == "POST" ) {
        _status = "200";
        _statusMsg = " OK";
    }
    else if ( _reqType == "POST" ) {
        _status = "204";
        _statusMsg = " No Content";        
    }
};

void ResponseBuilder::saveDateTime( void ) {
    
    std::time_t rawtime;
    std::tm* timeinfo;
    char buffer[ 80 ];

    std::time( &rawtime );
    timeinfo = std::localtime( &rawtime );

    // Format time as string
    std::strftime( buffer, 80, "%Y-%m-%d %H:%M:%S", timeinfo );
    std::string str( buffer );
    _dateTime = str;
};

void ResponseBuilder::buildBody( void ) {

    std::stringstream buffer;
    std::string filename;

    if ( _reqType == "GET") {

        filename = "." + _path;
        std::ifstream file( filename );

        if (file.is_open()) {
            buffer << file.rdbuf();
        } else {
            std::cout << "Unable to open file\n";
        }
        file.close();

        std::string bufString = buffer.str();
        if ( bufString.size() ) {
            _respBody = bufString;
        }
        else {
            _respBody = "EMPTY\n";
        }
    }
    else if ( _reqType == "POST" ) {

        std::cout << "POST received" << std::endl;
        _respBody = "DATA POSTED";
    }
    else if ( _reqType == "DELETE" ) {

        std::cout << "DELETE received" << std::endl;
        _respBody = "DATA DELETED";
    }
    
    std::stringstream ss;
    ss << _respBody.length();
    _contLen = ss.str();
};

void ResponseBuilder::writeToSocket( void ) {

    std::string full( _respHeader + _respBody );
    write( _socket, (full.c_str()), full.length() );    
};

void ResponseBuilder::printHeaderInfo( void ) {

    // std::cout << "_reqType: " << _reqType << std::endl;
    // std::cout << "requested path: " << _path << std::endl;
    // std::cout << "server: " << _serverName << std::endl;
    // std::cout << "content type: " << _contType << std::endl;
    // std::cout << "content len: " << _contLen << std::endl;
    // std::cout << "_time: " << _dateTime << std::endl;
    
    // complete header
    std::cout << _respHeader;
};

void ResponseBuilder::printBody( void ) {

    std::cout << "BODY: " << _respBody << std::endl;
};
