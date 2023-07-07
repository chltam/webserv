#include "../includes/ResponseBuilder.hpp"

ResponseBuilder::ResponseBuilder( vector<pair<string, string>> headerPairs, string body ) {

    _reqHeaderPairs = headerPairs;
    _reqBody = body;
};

ResponseBuilder::~ResponseBuilder() {};

void ResponseBuilder::fillReqInfo() {

    for (vector< pair<string, string> >::const_iterator it = _reqHeaderPairs.begin();
        it != _reqHeaderPairs.end(); ++it) {
        
        if ( it->first == "request type" ) {
            _reqType = it->second;
        }
        else if ( it->first == "path" ) {
            _path = it->second;
        }
        else if ( it->first == "Host" ) {
            _serverName = it->second;
        }
        else if ( it->first == "Accept" ) {
            _contType = it->second;
        }
    };
};

static AResponse* makeGetResponse( string path, string serverName, string contType, string reqBody ) {
	    return (new GetResponse( path, serverName, contType, reqBody ));
    }

static AResponse* makePostResponse( string path, string serverName, string contType, string reqBody ) {
    return (new PostResponse( path, serverName, contType, reqBody ));
}

static AResponse* makeDeleteResponse( string path, string serverName, string contType, string reqBody ) {
    return (new DeleteResponse( path, serverName, contType, reqBody ));
}

static AResponse* makeErrorResponse( string path, string serverName, string contType, string reqBody ) {
    return (new ErrorResponse( path, serverName, contType, reqBody ));
}

AResponse* ResponseBuilder::createResponse() {

    AResponse* ( *allResponses[] )( string _path, string _serverName, string _contType,
        string _reqBody ) = { &makeGetResponse , &makePostResponse , &makeDeleteResponse, &makeErrorResponse};
    string responses[] = { "GET", "POST", "DELETE" };

    for ( int i = 0; i < VALID_REQUEST_NUM; i++ ) {

        if ( _reqType == responses[ i ] ) {

            cout << _reqType << "Response created" << endl;
			return ( allResponses[ i ]( _path, _serverName, _contType, _reqBody )); // call creator ft here
        }
    }
    cout << _reqType << "Response could not be created" << endl;
    return ( makeErrorResponse( _path, _serverName, _contType, _reqBody ));
};
