#include "../includes/DeleteResponse.hpp"

DeleteResponse::DeleteResponse(string path,  string header, string reqBody):
    AResponse(path, header, reqBody ) {

};

DeleteResponse::~DeleteResponse() {};

int DeleteResponse::exec() {

    stringstream lenStr;
    string pathStr;

    // string fileDir = _path;
    // if ( fileDir.compare(0, 5, _path, 0, 5) != 0 ) {

    //     _respBody = "ACCESS FORBIDDEN\n";
    //     _status = "403 Forbidden";
    //     // lenStr << _respBody.length();
    //     //  _contLen = lenStr.str();
    //     return ( EXIT_FAILURE );
    // }

    cout << "DELETE received for " << _path << endl;

    if ( remove( _path.c_str()) != 0 ) {

        _respBody = "DATA NOT DELETED\n";
        _status = "404 Not Found";
        // lenStr << _respBody.length();
        //  _contLen = lenStr.str();
        return ( EXIT_FAILURE );

    } else {

        _respBody = "DATA DELETED\n";
        _status = "200 OK";
        // lenStr << _respBody.length();
        // _contLen = lenStr.str();
        return ( EXIT_SUCCESS );
    }
};
