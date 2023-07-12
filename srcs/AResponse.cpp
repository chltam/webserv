#include "../includes/AResponse.hpp"

AResponse::AResponse(string path, string header, string reqBody ):
    _path (path), _header( header ), _reqBody( reqBody ) {};

AResponse::~AResponse() {};

void AResponse::fillResponse() {

    _execResult = exec();
    if ( _execResult == 0 ) { // exec success
        _response = _header + _respBody;
    }
}

// void AResponse::saveDateTime() {

//     time_t rawtime;
//     tm* timeinfo;
//     char buffer[ 80 ];

//     time( &rawtime );
//     //  All HTTP date/time stamps MUST be represented in Greenwich Mean Time (GMT)!!!
//     timeinfo = gmtime( &rawtime );

//     strftime( buffer, sizeof(buffer), "%a, %d %b %Y %T %Z", timeinfo );
//     string str( buffer );
//     _dateTime = str;
// };

void AResponse::printHeaderInfo() {

    cout << _header ;
};

void AResponse::printBody() {

    cout << "BODY: " << _respBody << endl;
};

string AResponse::getResponse() {

    return ( _response );
}

int AResponse::getExecResult() {

    return ( _execResult );
};

string AResponse::getStatus() {

    return ( _status );
};
