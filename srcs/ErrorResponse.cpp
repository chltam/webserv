#include "ErrorResponse.hpp"

// one more field for the status
ErrorResponse::ErrorResponse( string path, string serverName, string contType, string reqBody, string status ):
    AResponse( path, serverName, contType, reqBody ) {

    _status = status;
};

ErrorResponse::~ErrorResponse() {};

int ErrorResponse::exec() {

    stringstream lenStr;
    cout << "Error Response sent\n";

    _respBody = "<!doctype html><html><head><title>Error</title></head>\
<body><p>" + _status + "</p></body></html>";
    lenStr << _respBody.length();
    _contLen = lenStr.str();
    return ( EXIT_SUCCESS );
};
