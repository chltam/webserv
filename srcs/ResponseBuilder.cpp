#include "../includes/ResponseBuilder.hpp"
#include "ResponseBuilder.hpp"

using namespace std;

ResponseBuilder::ResponseBuilder() {};

ResponseBuilder::~ResponseBuilder() {};

Response* ResponseBuilder::createNewResponse(Request &request, const Config& config  )
{
    Response* response = new Response();
    response->_headerFields["Server"] = request.getHeaderValueFromKey("Host");

    std::string contType= request.getHeaderValueFromKey("Host");
    std::string::size_type pos = contType.find(',');
    response->_headerFields["Content-Type"] = contType.substr(0, pos); 

    response->setStatus(setResponseStatus(request,config,*response));

    return response;
}

int ResponseBuilder::setResponseStatus( Request& request, const Config& config, Response& response )
{
    const ConfigServer* server = config.getConfigServerFromRequest( request.getHeaderValueFromKey("Host") );

    string path = request.getHeaderValueFromKey( "path" );
    const ConfigRoute* configRoute = server->getRouteFromPath( path );
   
    if(configRoute == NULL){
        PRINT("ERROR, could find CONFIGROUTE, this should never happen!");
    }

    if(configRoute->m_shouldRedirect == true) {
        PRINT("REDIRECT SIR!");
        response._headerFields["Location"] = configRoute->m_redirectDir;
        return 301;
    }

    std::string newfullPath(configRoute->m_root + path);
    int ret1 = ValidatePath(newfullPath);
    int method = StringToMethodEnum(request.getHeaderValueFromKey("request type"));

    if(ret1 == -1 && method != METH_POST){
        PRINT("ERROR, Path is invalid!");
        response.setPath("./errorpages/error404.html");
        return 404;
    }
    else if(ret1 == S_IFDIR && method == METH_GET) {
        //only check this for a get request
            if(newfullPath[newfullPath.length()-1] != '/' ){
                newfullPath += "/";
                newfullPath += configRoute->m_defaultFile;
            }
            else
                newfullPath += configRoute->m_defaultFile;
            ret1 = ValidatePath(newfullPath);
            if(ret1 != S_IFREG) {
                PRINT("ERROR, Path is invalid!");
                response.setPath("./errorpages/error404.html");
                return 404;
            }
        }

    if(!(method & configRoute->m_allowedMethods)){ //if you are not allowed to access the resource with GET POST or DELETE
        PRINT("ERROR,you have no rights to access this resource with the Method provided");
        response.setPath("./errorpages/error403.html");
        return 403;
    }

    if(method == METH_DELETE){

        // if path is root on delete, send which response?
        deleteResource(newfullPath);
    }
    else if(method == METH_POST){
        uploadResource(newfullPath,request.getBody());
    }
    else //only for get
        response.setPath(newfullPath);
    return 200;
}



void ResponseBuilder::deleteResource(const std::string& newfullPath)
{
    remove( newfullPath.c_str());
}

void ResponseBuilder::uploadResource(const std::string& newfullPath,std::string resourceData)
{
    string filename;
    stringstream lenStr;

    cout << "POST received" << endl;

    filename = newfullPath;
    ofstream file( filename, std::ios::app);

    if ( file.is_open() ) {

        file << resourceData;

    } else {
        cout << "Unable to open file\n";
        // _status = "403 Not Allowed";
    }
    file.close();
}