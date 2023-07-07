#include "GetResponse.hpp"

GetResponse::GetResponse( string path, string serverName, string contType, string reqBody ):
    AResponse( path, serverName, contType, reqBody ) {

};

GetResponse::~GetResponse() {};

int GetResponse::exec() {

    stringstream buffer;
    stringstream lenStr;
    string filename;
    string bufString;

    filename = "./files/" + _path;
    ifstream file( filename );

    if ( file.is_open() ) {

        buffer << file.rdbuf();

    } else {

        cout << "Unable to open file\n";
        _respBody = "COULD NOT OPEN FILE\n";
        lenStr << _respBody.length();
        _contLen = lenStr.str();
        return ( EXIT_FAILURE );
    }
    file.close();

    bufString = buffer.str();
    if ( bufString.size() ) {
        _respBody = bufString;
    }
    else {
        _respBody = "EMPTY FILE\n";
    }

    lenStr << _respBody.length();
    _contLen = lenStr.str();

    return ( EXIT_SUCCESS );
};
