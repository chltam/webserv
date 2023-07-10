#include "../includes/AResponse.hpp"

AResponse::AResponse( string path, string serverName, string contType, string reqBody ):
    _path( path ), _serverName( serverName ), _contType( contType ), _reqBody( reqBody ) {

};

AResponse::~AResponse() {};

void AResponse::fillResponse() {

    exec();
    buildHeader();

    _response = _respHeader + _respBody;
}

void AResponse::buildHeader() {
    
    saveDateTime();

    // CONTENT TYPE is hard coded because Firefox sends too much info (more parsing needed)
    _respHeader = "HTTP/1.1 " + _status + /* "\r\nConnection: keep-alive" + */ "\r\nContent-Type: text/html"\
        + "\r\nContent-Length: " + _contLen + "\r\nDate: " + _dateTime + "\r\nServer: " + _serverName + "\r\n\r\n";
}

// 1xx informational response – the request was received, continuing process
// 2xx successful – the request was successfully received, understood, and accepted
// 3xx redirection – further action needs to be taken in order to complete the request
// 4xx client error – the request contains bad syntax or cannot be fulfilled
// 5xx server error – the server failed to fulfil an apparently valid request

void AResponse::saveDateTime() {
    
    time_t rawtime;
    tm* timeinfo;
    char buffer[ 80 ];

    time( &rawtime );
    //  All HTTP date/time stamps MUST be represented in Greenwich Mean Time (GMT)!!!
    timeinfo = localtime( &rawtime );

    // HTTP also requires the weekday in front! This is preferred: Sun, 06 Nov 1994 08:49:37 GMT
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
