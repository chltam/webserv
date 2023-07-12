#include "ErrorResponse.hpp"

// one more field for the status
ErrorResponse::ErrorResponse(string path,  string header, string reqBody):
    AResponse(path, header, reqBody ) {

};

ErrorResponse::~ErrorResponse() {};

int ErrorResponse::exec() {

    stringstream lenStr;
    cout << "Error Response sent\n";

    _respBody = "<!doctype html><html><head><title>Error</title></head>\
<body><p>" + _status + "</p></body></html>";
    
    return ( EXIT_SUCCESS );
};
