#include "../includes/GetResponse.hpp"

GetResponse::GetResponse(string path,  string header, string reqBody):
    AResponse(path, header, reqBody ) {};

GetResponse::~GetResponse() {};

int GetResponse::exec() {

    stringstream buffer;
    stringstream lenStr;
    string filename;
    string bufString;

    filename = _path;

    ifstream file( filename );

    if ( file.is_open() ) {

        buffer << file.rdbuf();
        _status = "200 OK";
    }
    file.close();

    bufString = buffer.str();
    if ( bufString.size() ) {
        _respBody = bufString;
    }
    else {
        _respBody = "EMPTY FILE\n";
    }

    return ( EXIT_SUCCESS );
};
