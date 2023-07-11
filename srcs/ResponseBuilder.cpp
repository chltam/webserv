#include "../includes/ResponseBuilder.hpp"

using namespace std;

ResponseBuilder::ResponseBuilder() {};

ResponseBuilder::~ResponseBuilder() {};

void ResponseBuilder::fillReqInfo( Request& request, const Config& config ) {

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

            cerr << "IT->SECOND: " << it->second << endl;
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
};

void ResponseBuilder::buildPath( Request& request, const Config& config ) {

    const ConfigServer* server = config.getConfigServerFromRequest( request.getHeaderValueFromKey("Host") );
	// cerr << server << endl;

    string path = request.getHeaderValueFromKey( "path" );
    // cerr << "path from request: " << path << endl;
    const ConfigRoute* configRoute = server->getRouteFromPath( path );
    // cerr << "HEEEEEEEEEEEEEEE" << endl;
    // cerr << *configRoute << endl;

    if(configRoute->m_shouldRedirect == true) {
        PRINT("REDIRECT SIR!");
        PRINTVAR(configRoute->m_redirectDir);
    }
    // if()

    std::string fullpath(configRoute->m_root + path);
    PRINTVAR(fullpath);

    std::string tempPath(configRoute->m_root);
    int ret;
    size_t prev = 0;
    while(tempPath != fullpath){
        ret = ValidatePath(tempPath);
        if(ret == -1) {
            PRINT("PATH IS INVALID -> ERROR RESPONSE");
        }
        else if(ret == S_IFREG){
            PRINT("FOUND REGULAR FILE, THIS SHOULD BE THE END");
            //do more checks for CGI as rest could be params
            break;
        }
        else if(ret == S_IFDIR){
            //append next
            prev = path.find("/",prev+1);
            PRINTVAR(prev);
            if(prev != std::string::npos){
                tempPath = fullpath.substr(0,prev);
            }
            else
                tempPath = fullpath;
            continue;
        }
        else{
            PRINT("FOUND SOMETHING THATS NEITHER A DIR OR FILE, more checking?");
            break;
        }
    }
    if(tempPath == fullpath){
        PRINT("REQUEST IS VALID DIRECTORY, APPEND index");
        fullpath += configRoute->m_defaultFile;
        ret = ValidatePath(tempPath);

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

static AResponse* makeErrorResponse( string path, string serverName, string contType, string reqBody ) {
    return (new ErrorResponse( path, serverName, contType, reqBody ));
}

AResponse* ResponseBuilder::createResponse(Request& request, const Config& config) {

    // fillReqInfo stuff here
    fillReqInfo(request,config);
    buildPath(request,config); //currently used for jean testing

    AResponse* ( *allResponses[] )( string _path, string _serverName, string _contType,
        string _reqBody ) = { &makeGetResponse , &makePostResponse , &makeDeleteResponse };
    string responses[] = { "GET", "POST", "DELETE" };

    for ( int i = 0; i < VALID_REQUEST_NUM; i++ ) {

        if ( _reqType == responses[ i ] ) {

            cout << _reqType << "Response created" << endl;
			return ( allResponses[ i ]( _path, _serverName, _contType, request.getBody() )); // call creator ft here
        }
    }
    cout << _reqType << "Response could not be created" << endl;
    return ( makeErrorResponse( _path, _serverName, _contType, request.getBody() ));
};
