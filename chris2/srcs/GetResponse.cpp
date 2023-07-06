#include "GetResponse.hpp"

GetResponse::GetResponse( string path, string serverName, string contType, string reqBody ):
    AResponse( path, serverName, contType, reqBody ) {

};

GetResponse::~GetResponse() {};

void GetResponse::buildBody() {

    stringstream buffer;
    string filename;
    string bufString;

    filename = "." + _path;
    ifstream file( filename );

    if (file.is_open()) {
        buffer << file.rdbuf();
    } else {
        cout << "Unable to open file\n";
    }
    file.close();

    bufString = buffer.str();
    if ( bufString.size() ) {
        _respBody = bufString;
    }
    else {
        _respBody = "EMPTY\n";
    }

    // else if ( _reqType == "POST" ) {

    //     cout << "POST received" << endl;
    //     _respBody = "DATA POSTED";
    // }
    // else if ( _reqType == "DELETE" ) {

    //     cout << "DELETE received" << endl;
    //     _respBody = "DATA DELETED";
    // }
    
    stringstream ss;
    ss << _respBody.length();
    _contLen = ss.str();
};