#include "ErrorResponse.hpp"

ErrorResponse::ErrorResponse( string path, string serverName, string contType, string reqBody ):
    AResponse( path, serverName, contType, reqBody ) {

};

ErrorResponse::~ErrorResponse() {};

int ErrorResponse::exec() {

    cout << "Error Response sent\n";

    _status = "501 Not Implemented";
    _contLen = "0";
    return ( EXIT_SUCCESS );
};
