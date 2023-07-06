#include "AResponse.hpp"

AResponse::AResponse( string path, string serverName, string contType, string reqBody ):
    _path( path ), _serverName( serverName ), _contType( contType ), _reqBody( reqBody ) {

};

AResponse::~AResponse() {};

void AResponse::buildResponse() {

    buildHeader();
    buildBody();

    // CONTENT TYPE is hard coded because Firefox sends too much info (more parsing needed)
    _respHeader = "HTTP/1.1 " + _status + _statusMsg + /* "\r\nConnection: keep-alive" + */ "\r\nContent-Type: text/html"\
        + "\r\nContent-Length: " + _contLen + "\r\nDate: " + _dateTime + "\r\nServer: " + _serverName + "\r\n\r\n";

    _response = _respHeader + _respBody;
}

void AResponse::buildHeader() {
    
    saveDateTime();
    determineStatus();
}

void AResponse::determineStatus() {

    // how is it determined?
    _status = "200";
    _statusMsg = " OK";
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

void AResponse::saveDateTime() {
    
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

void AResponse::printHeaderInfo() {

    cout << _respHeader;
};

void AResponse::printBody() {

    cout << "BODY: " << _respBody << endl;
};

string AResponse::getResponse() {

    return ( _response );
}
