#include "../includes/ResponseBuilder.hpp"
#include "../includes/MetaVars.hpp"

ResponseBuilder::ResponseBuilder( vector<pair<string, string> > headerPairs, string body, char **envp ) {

    _reqHeaderPairs = headerPairs;
    _reqBody = body;
	_envp = envp;
};

ResponseBuilder::~ResponseBuilder() {};

void ResponseBuilder::fillReqInfo() {

	MetaVars mvars(_envp);
    for (vector< pair<string, string> >::const_iterator it = _reqHeaderPairs.begin();
        it != _reqHeaderPairs.end(); ++it) {
        
        if ( it->first == "request type" ) {
            _reqType = it->second;
			mvars.set_value("REQUEST_METHOD", it->second);
        }
        else if ( it->first == "path" ) {
            _path = it->second;
			mvars.set_value("PATH_INFO", it->second);
        }
        else if ( it->first == "Host" ) {
            _serverName = it->second;
			mvars.set_value("SERVER_NAME", it->second.substr(0, it->second.find(':')));
			mvars.set_value("SERVER_PORT", it->second.substr(it->second.find(':'), it->second.size()));
        }
        else if ( it->first == "Accept" ) {
            _contType = it->second;
			mvars.set_value("CONTENT_TYPE", it->second);
        }
    };
	mvars.set_value("SERVER_PROTOCOL", "HTTP/1.1");
	mvars.set_value("SCRIPT_FILENAME", "files/simple.html");
	mvars.set_content_length(_reqBody);
	mvars.update_envp();
	mvars.print_envp();

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
