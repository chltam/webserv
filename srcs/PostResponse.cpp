#include "../includes/PostResponse.hpp"

PostResponse::PostResponse(string path,  string header, string reqBody):
    AResponse(path, header, reqBody ) {};

PostResponse::~PostResponse() {};

int PostResponse::exec() {

    string filename;
    stringstream lenStr;

    cout << "POST received" << endl;

    filename = _path;
    ofstream file( filename, std::ios::app);

    PRINT_MORE(_path);

    if ( file.is_open() ) {

        file << _reqBody;
        _respBody = "DATA POSTED";
        _status = "201 Created";
        // lenStr << _respBody.length();
        // _contLen = lenStr.str();
        return ( EXIT_SUCCESS );

    } else {
        cout << "Unable to open file\n";
        _status = "403 Not Allowed";
        // _contLen = lenStr.str();
        return ( EXIT_FAILURE );
    }
    file.close();

    return ( EXIT_SUCCESS );
};
