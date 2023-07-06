#include "ResponseBuilder.hpp"

ResponseBuilder::ResponseBuilder( int socket, vector<pair<string, string>> headerPairs, string body ) {

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

    string header;
    
    for (vector<pair<string, string> >::const_iterator it = _reqHeaderPairs.begin();
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
    if ( _reqType == "GET" || _reqType == "POST" || _reqType == "DELETE") {
        _status = "200";
        _statusMsg = " OK";
    }
    // else if ( _reqType == "DELETE" ) {
        // ( if ( FILE IS FOUND )) {  // request successful
            // _status = "204";
            // _statusMsg = " No Content";        
        // }
        /* else { // file not found
            _status = "404";
            _statusMsg = " Not Found";
        } */
    // }
};

void ResponseBuilder::saveDateTime( void ) {
    
    time_t rawtime;
    tm* timeinfo;
    char buffer[ 80 ];

    time( &rawtime );
    timeinfo = localtime( &rawtime );

    // Format time as string
    strftime( buffer, 80, "%Y-%m-%d %H:%M:%S", timeinfo );
    string str( buffer );
    _dateTime = str;
};

void ResponseBuilder::buildBody( void ) {

    stringstream buffer;
    string filename;

    if ( _reqType == "GET") {

        filename = "." + _path;
        ifstream file( filename );

        if (file.is_open()) {
            buffer << file.rdbuf();
        } else {
            cout << "Unable to open file\n";
        }
        file.close();

        string bufString = buffer.str();
        if ( bufString.size() ) {
            _respBody = bufString;
        }
        else {
            _respBody = "EMPTY\n";
        }
    }
    else if ( _reqType == "POST" ) {

        cout << "POST received" << endl;
        _respBody = "DATA POSTED";
    }
    else if ( _reqType == "DELETE" ) {

        cout << "DELETE received" << endl;
        _respBody = "DATA DELETED";
    }
    
    stringstream ss;
    ss << _respBody.length();
    _contLen = ss.str();
};

void ResponseBuilder::writeToSocket( void ) {

    string full( _respHeader + _respBody );
    write( _socket, (full.c_str()), full.length() );    
};

void ResponseBuilder::printHeaderInfo( void ) {

    cout << _respHeader;
};

void ResponseBuilder::printBody( void ) {

    cout << "BODY: " << _respBody << endl;
};
