#include "../includes/AResponse.hpp"

AResponse::AResponse( string path, string serverName, string contType, string reqBody ):
    _path( path ), _serverName( serverName ), _contType( contType ), _reqBody( reqBody ) {};

AResponse::~AResponse() {};

void AResponse::fillResponse() {

    _execResult = exec();
    if ( _execResult == 0 ) { // exec success
        buildHeader();
        _response = _respHeader + _respBody;
    }
}

void AResponse::buildHeader() {
    
    saveDateTime();

    // CONTENT TYPE is hard coded because Firefox sends too much info (more parsing needed)
    _respHeader = "HTTP/1.1 " + _status + /* "\r\nConnection: keep-alive" + */ "\r\nContent-Type: " + _contType\
        + "\r\nContent-Length: " + _contLen + "\r\nDate: " + _dateTime + "\r\nServer: " + _serverName + "\r\n\r\n";
}

void AResponse::saveDateTime() {
    
    time_t rawtime;
    tm* timeinfo;
    char buffer[ 80 ];

    time( &rawtime );
    //  All HTTP date/time stamps MUST be represented in Greenwich Mean Time (GMT)!!!
    timeinfo = gmtime( &rawtime );

    strftime( buffer, sizeof(buffer), "%a, %d %b %Y %T %Z", timeinfo );
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

int AResponse::getExecResult() {

    return ( _execResult );
};

string AResponse::getStatus() {

    return ( _status );
};