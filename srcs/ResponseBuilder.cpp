#include "../includes/ResponseBuilder.hpp"

// ResponseBuilder::ResponseBuilder( vector<pair<string, string>> headerPairs, string body ) {
ResponseBuilder::ResponseBuilder() {

};

ResponseBuilder::~ResponseBuilder() {};

void ResponseBuilder::fillReqInfo(Request& request, const Config& config) {

    //find server based on port
    //check the directory (valid route) -> if not exist == error
        //use open dir
    //if file in directory if file exists? + access rights
    //check if GET/POST/DELETE etc is allowed? -> if not == error
    //do we need to check what the client accepts?
    //check actual file for extensions
        //if CGI valid -> run CGI
        //else do the rest her


	const ConfigServer& server = config.getConfigServerFromRequest(request.getHeaderValueFromKey("Host"));
	std::cerr << server << std::endl;

    std::string path = request.getHeaderValueFromKey("path");
    std::cerr << path << std::endl;

    // if (!checkIfFolder(path))
        // --> it's a file!

    DIR* dir; // DIR is a type from dirent.h
    struct dirent* ent; // dirent is a struct type from dirent.h

    dir = opendir((server.m_root + path).c_str()); // replace with your directory
    if (dir == NULL) { // always check if opendir was successful
        std::cerr << "NOT A DIR" << std::endl;
        // return ;
    } else {

        std::cerr << "IT IS A DIR" << std::endl;
    }


    std::string tempPath(path.substr(0,path.rfind('/')));
    std::cerr << "temp path =" << tempPath << std::endl;
    // /files/test_dir ->loop over the routes
    const ConfigRoute& configRoute = server.getRouteFromPath(tempPath);
    //if(!validDirectory(path))
        //error handling
    std::string completePath(configRoute.m_root + tempPath);
        std::cerr << "compelte path =" << completePath << std::endl;

    std::cerr << "HEEEEEEEEEEEEEEE" << std::endl;
    std::cerr << configRoute << std::endl;

    vector<pair<string, string>> reqHeaderPairs = request.getHeaderPairs(); 

    for (vector< pair<string, string> >::const_iterator it = reqHeaderPairs.begin();
        it != reqHeaderPairs.end(); ++it) {
        
        if ( it->first == "request type" ) {
            _reqType = it->second;
        }
        else if ( it->first == "path" ) { // COULD BE another function that resolves the path
            
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

AResponse* ResponseBuilder::createResponse(Request& request, const Config& config) {

    // fillReqInfo stuff here
    fillReqInfo(request,config);


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
