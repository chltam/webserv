#include "DeleteResponse.hpp"

DeleteResponse::DeleteResponse( string path, string serverName, string contType, string reqBody ):
    AResponse( path, serverName, contType, reqBody ) {

};

DeleteResponse::~DeleteResponse() {};

int DeleteResponse::exec() {

    stringstream lenStr;
    string pathStr;

    pathStr = "." + _path;
    cout << "DELETE received for " << pathStr << endl;

    if ( remove( pathStr.c_str()) != 0 ) {
        
        _respBody = "DATA NOT DELETED\n";
        lenStr << _respBody.length();
         _contLen = lenStr.str();
        return ( EXIT_FAILURE );

    } else {

        _respBody = "DATA DELETED\n";
        lenStr << _respBody.length();
        _contLen = lenStr.str();
        return ( EXIT_SUCCESS );
    }
};
