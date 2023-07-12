#include "../includes/ResponseBuilder.hpp"
#include "ResponseBuilder.hpp"

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


    // give 301 Code (Moved Permanently) here? Server responds with the new url.
    // Once a client receives that request he will than make a new http request from that location.
    _header["Status"] = "200 OK";
    _header["Date"] = saveDateTime();
    vector<pair<string, string>> reqHeaderPairs = request.get_request_pair();

    for (vector< pair<string, string> >::const_iterator it = reqHeaderPairs.begin();
        it != reqHeaderPairs.end(); ++it) {

        if ( it->first == "request type" ) {
            _reqType = it->second;
        }
        else if ( it->first == "Host" ) {
            _serverName = it->second;
            _header["Server"] = it->second;
        }
        else if ( it->first == "Accept" ) {
            _contType = it->second;     // media-type = type "/" subtype * (NO whitespace between)
            _header["Content-Type"] = it->second;
            // _header["Content-Type"] = "text/html";
        }
    };
	int ret = buildPath( request, config);

    _header["Content-Length"] = std::to_string(getFileSize(_path));
    if(_header["Content-Length"] == "-1")
        _header["Content-Length"] = "0";

    return ( ret );
};

int ResponseBuilder::buildPath( Request& request, const Config& config ) {

    const ConfigServer* server = config.getConfigServerFromRequest( request.getHeaderValueFromKey("Host") );
	// cerr << server << endl;

    string path = request.getHeaderValueFromKey( "path" );
    // cerr << "path from request: " << path << endl;
    const ConfigRoute* configRoute = server->getRouteFromPath( path );
    // cerr << "HEEEEEEEEEEEEEEE" << endl;
    // cerr << *configRoute << endl;
    if(configRoute == NULL){
        PRINT("ERROR, could find CONFIGROUTE, this should never happen!");
    }

    if(configRoute->m_shouldRedirect == true) {
        PRINT("REDIRECT SIR!");
        _header["Location"] = configRoute->m_redirectDir;
        _header["Status"] = "301 Moved Permanently";
        PRINTVAR(configRoute->m_redirectDir);
        return 2;
    }
    // if()

    std::string fullpath(configRoute->m_root + path);
    PRINTVAR(fullpath);

    std::string tempPath(configRoute->m_root);
    PRINTVAR(tempPath);
    int ret;
    size_t prev = tempPath.length() -1;
    while(true){
        ret = ValidatePath(tempPath);
        if(ret == -1) {
            PRINT("PATH IS INVALID -> ERROR RESPONSE");
            break;
        }
        else if(ret == S_IFREG){
            PRINT("FOUND REGULAR FILE, THIS SHOULD BE THE END");
            fullpath = configRoute->m_root + tempPath;
            break;
        }
        else if(ret == S_IFDIR){
            //append next
            PRINT("VAR is DIRECTORY");
            prev = fullpath.find("/",prev+1);
            PRINTVAR(prev);
            if(prev != std::string::npos){
                tempPath = fullpath.substr(0,prev);
            }
            else{
                tempPath = fullpath;
                PRINTVAR(tempPath);
                ret = ValidatePath(tempPath);
                break;
            }
            PRINTVAR(tempPath);
            continue;
        }
        else{
            PRINT("FOUND SOMETHING THATS NEITHER A DIR OR FILE, more checking?");
            break;
        }
    }
    PRINT("AFTER LOOP");

    if(ret == -1){
        PRINT("CREATE ERROR RESPONSE");
    }
    else if(ret == S_IFDIR){
        PRINT("REQUEST IS VALID DIRECTORY, APPEND index");
        if(tempPath[tempPath.length()-1] != '/' ){
            fullpath += "/";
            fullpath += configRoute->m_defaultFile;
        }
        else
            fullpath += configRoute->m_defaultFile;
        PRINTVAR(fullpath);
        ret = ValidatePath(fullpath);
        if(ret != S_IFREG){
            PRINT("CREATE ERROR RESPONSE, default file is not a file, or non existent");
        }
    }

    //just for debugging checking if the final result has been captured correctly
    PRINT("");
    PRINT("");
    if(ret != S_IFREG){
        PRINT("AN ERROR OCCURED, COULDNT FIND FILE");
        _status = "403 File not found";
        return (EXIT_FAILURE);
    }

    else{
        PRINT("FINAL PATH AFTER ALL TRANSFORMATION:");
        PRINTVAR(fullpath);
        _path = fullpath;
    }
    PRINT("");
    PRINT("");

    return (EXIT_SUCCESS);
    //QUESTION
    // what happens if the client tries to access a file that is not part of the routes? will we take the global config or is this not allowed?

};

std::string ResponseBuilder::headerToString()
{
    // _respHeader = "HTTP/1.1 " + _status + /* "\r\nConnection: keep-alive" + */ "\r\nContent-Type: " + "text/html"\
    //     + "\r\nContent-Length: " + _contLen + "\r\nDate: " + _dateTime + "\r\nServer: " + _serverName + "\r\n\r\n";

    std::string ret("HTTP/1.1 " + _header["Status"]+ "\r\nConnection: keep-alive\r\n");

    std::map<std::string,std::string>::const_iterator it = _header.begin();
    for (; it != _header.end(); it++) {
        if(it->first != "Status"){
            ret += it->first + ": " + it->second + "\r\n";
        }
    }
    ret += "\r\n";

    return ret;
}
static AResponse *makeGetResponse(std::string path,std::string header, string reqBody)
{
    return (new GetResponse(path,header, reqBody));
}

static AResponse* makePostResponse(std::string path, std::string header, string reqBody ) {
    return (new PostResponse(path,header, reqBody ));
}

static AResponse* makeDeleteResponse(std::string path, std::string header, string reqBody ) {
    return (new DeleteResponse(path,header, reqBody ));
}

static AResponse* makeErrorResponse(std::string path, std::string header, string reqBody) {
    return (new ErrorResponse(path, header, reqBody));
}

static AResponse* makeRedirResponse(std::string path, std::string header, string reqBody) {
    return (new RedirResponse(path, header, reqBody));
}

AResponse* ResponseBuilder::createResponse(Request& request, const Config& config) {

    int checker = fillReqInfo(request,config);

    if ( checker == EXIT_FAILURE ) {
       return makeErrorResponse (_path, headerToString(), request.getBody());
    }
    else if(checker == 2){
        PRINT("ENTERED REDIR REPSONSE ");
       return makeRedirResponse (_path, headerToString(), request.getBody());

    }
    AResponse* ( *allResponses[] )( std::string path, string _header,
        string _reqBody ) = { &makeGetResponse , &makePostResponse , &makeDeleteResponse };
    string responses[] = { "GET", "POST", "DELETE" };

    for ( int i = 0; i < VALID_REQUEST_NUM; i++ ) {

        if ( _reqType == responses[ i ] ) {

            AResponse* response = allResponses[ i ](_path, headerToString(), request.getBody() );
			if ( response->getExecResult() == 1 ) {

                string status = response->getStatus();
                cout << _reqType << "Response could not be created" << endl;
                delete response;
                return ( makeErrorResponse(_path, headerToString(), request.getBody()));

            } else {
                cout << _reqType << "Response created" << endl;
                return ( response );
            }
        }
    }
    cout << _reqType << "Response could not be created" << endl;
    _header["Status"] = "501 Not Implemented";
    return ( makeErrorResponse(_path, headerToString(), request.getBody()));
};
