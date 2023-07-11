#include "../includes/PostResponse.hpp"

PostResponse::PostResponse( string path, string serverName, string contType, string reqBody ):
    AResponse( path, serverName, contType, reqBody ) {};

PostResponse::~PostResponse() {};

int PostResponse::exec() {

    string filename;
    stringstream lenStr;

    cout << "POST received" << endl;

    filename = "./files/" + _path;
    ofstream file( filename );

    if ( file.is_open() ) {

        file << _reqBody;
        _respBody = "DATA POSTED";
        _status = "201 Created";
        lenStr << _respBody.length();
        _contLen = lenStr.str();
        return ( EXIT_SUCCESS );

    } else {
        cout << "Unable to open file\n";
        _status = "403 Not Allowed";
        _contLen = lenStr.str();
        return ( EXIT_FAILURE );
    }
    file.close();
};
