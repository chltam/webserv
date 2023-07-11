#include "../includes/ResponseBuilder.hpp"

using namespace std;

ResponseBuilder::ResponseBuilder() {};

ResponseBuilder::~ResponseBuilder() {};

int ResponseBuilder::fillReqInfo( Request& request, const Config& config ) {

    //find server based on port
    //check the directory (valid route) -> if not exist == error
        //use open dir
    //if file in directory if file exists? + access rights
    //check if GET/POST/DELETE etc is allowed? -> if not == error
    //do we need to check what the client accepts?
    //check actual file for extensions
        //if CGI valid -> run CGI
        //else do the rest her

	// buildPath( request, config);

    // give 301 Code (Moved Permanently) here? Server responds with the new url.
    // Once a client receives that request he will than make a new http request from that location. 

    vector<pair<string, string>> reqHeaderPairs = request.getHeaderPairs(); 

    for (vector< pair<string, string> >::const_iterator it = reqHeaderPairs.begin();
        it != reqHeaderPairs.end(); ++it) {
        
        if ( it->first == "request type" ) {
            _reqType = it->second;
        }
        else if ( it->first == "path" ) { // SHOULD BE REPLACED by buildPath()
            
            if ( it->second == "/" ) {
                _path = "/files/index.html";
            }
            else {
                _path = it->second;
            }
        }
        else if ( it->first == "Host" ) {
            _serverName = it->second;
        }
        else if ( it->first == "Accept" ) {
            _contType = it->second;     // media-type = type "/" subtype * (NO whitespace between)
        }
    };

    return ( EXIT_SUCCESS );
};

void ResponseBuilder::buildPath( Request& request, const Config& config ) {

    const ConfigServer& server = config.getConfigServerFromRequest( request.getHeaderValueFromKey("Host") );
	cerr << server << endl;
    // check here? if no server/config route -> ERROR

    string path = request.getHeaderValueFromKey( "path" );
    cerr << "path from request: " << path << endl;

    // if ( !checkIfDir( server, path) ) {
    //     // do sth to path string?
    // }

    string tempPath( path.substr( 0,path.rfind('/') ));
    cerr << "temp path =" << tempPath << endl;
    // /files/test_dir ->loop over the routes

    string completePath;
    if ( tempPath.length() != 0 ) {

        const ConfigRoute& configRoute = server.getRouteFromPath( tempPath );
        completePath = configRoute.m_root + tempPath;
        cerr << "complete path =" << completePath << endl;

        cerr << "HEEEEEEEEEEEEEEE" << endl;
        cerr << configRoute << endl;
    }
};

bool ResponseBuilder::checkIfDir( const ConfigServer& server, const string& path ) {

    DIR* dir;
    struct dirent* ent;

    dir = opendir((server.m_root + path).c_str());
    if (dir == NULL) { 
        cerr << "NOT A DIR" << endl;
        return ( false );
    } else {
        cerr << "IT IS A DIR" << endl;
        return ( true );
    }
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

static AResponse* makeErrorResponse( string path, string serverName, string contType, string reqBody, string status ) {
    return (new ErrorResponse( path, serverName, contType, reqBody, status ));
}

AResponse* ResponseBuilder::createResponse(Request& request, const Config& config) {

    int checker = 0;

    if ( fillReqInfo(request,config) == EXIT_FAILURE ) {
        makeErrorResponse ( _path, _serverName, _contType, request.getBody(), _status);
    };

    AResponse* ( *allResponses[] )( string _path, string _serverName, string _contType,
        string _reqBody ) = { &makeGetResponse , &makePostResponse , &makeDeleteResponse };
    string responses[] = { "GET", "POST", "DELETE" };

    for ( int i = 0; i < VALID_REQUEST_NUM; i++ ) {

        if ( _reqType == responses[ i ] ) {

            AResponse* response = allResponses[ i ]( _path, _serverName, _contType, request.getBody() );
			if ( response->getExecResult() == 1 ) {

                string status = response->getStatus();
                cout << _reqType << "Response could not be created" << endl;
                delete response;
                return ( makeErrorResponse( _path, _serverName, _contType, request.getBody(), status ));
            
            } else {
                cout << _reqType << "Response created" << endl;
                return ( response );
            }
        }
    }
    cout << _reqType << "Response could not be created" << endl;
    return ( makeErrorResponse( _path, _serverName, _contType, request.getBody(), "501 Not Implemented" ));
};
