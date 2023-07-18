#include "../includes/ResponseBuilder.hpp"
#include "ResponseBuilder.hpp"

using namespace std;

ResponseBuilder::ResponseBuilder() {};

ResponseBuilder::~ResponseBuilder() {};

Response* ResponseBuilder::createNewResponse(Request &request, const Config& config, MetaVars& mvars  )
{
    Response* response = new Response();

    if ( !request.getTimeout() ) {

        response->insertHeaderField("Server",request.getHeaderValueFromKey("Host"));

        std::string contType= request.getHeaderValueFromKey("Accept");
        std::string::size_type pos = contType.find(',');
        // response->_headerFields["Content-Type"] = contType; /*content type*/

        response->insertHeaderField("Content-Type",contType.substr(0, pos));
    }
    response->setStatus(setResponseStatus(request,config,*response,mvars));

    return response;
}

int ResponseBuilder::setResponseStatus( Request& request, const Config& config, Response& response, MetaVars& mvars )
{   
    if (request.getTimeout())
    {
        response.insertHeaderField("Server", "localhost"); // NEEDS TO BE CHANGED ACCORDING TO PORT ETC.
        response.insertHeaderField("Content-Type", "text/html");
        response.setPathFromErrorCode(408);
        return 408;
    }
    const ConfigServer* server = config.getConfigServerFromRequest( request.getHeaderValueFromKey("Host") );

    string path = request.getHeaderValueFromKey( "path" );
    const ConfigRoute* configRoute = server->getRouteFromPath( path );

    if(configRoute == NULL){
        PRINT("ERROR, could find CONFIGROUTE, this should never happen!");
    }

    if(configRoute->m_shouldRedirect == true) {
        PRINT("REDIRECT SIR!");
        response.insertHeaderField("Location",configRoute->m_redirectDir);
        return 301;
    }

    std::string newfullPath(configRoute->m_root + path);
    int ret1 = ValidatePath(newfullPath);
    int method = StringToMethodEnum(request.getHeaderValueFromKey("request type"));
    PRINTVAR(newfullPath);
    PRINTVAR(ret1);
    PRINTVAR(S_IFREG);
    PRINTVAR(S_IFDIR);
    if(ret1 == -1 && method != METH_POST){
        PRINT("ERROR, Path is invalid!");
        response.setPathFromErrorCode(404);
        return 404;
    }
    else if(ret1 == S_IFDIR && method == METH_GET) { //find,append,validate index file, if not, check for autoindex, else error
        //only check this for a get request
            std::string tempPath(newfullPath);
            if(newfullPath[newfullPath.length()-1] != '/' ){
                newfullPath += "/";
                newfullPath += configRoute->m_defaultFile;
            }
            else
                newfullPath += configRoute->m_defaultFile;
            ret1 = ValidatePath(newfullPath);
            PRINTVAR(ret1);
            if((ret1 == -1 || ret1 == S_IFDIR) && configRoute->m_autoindex == true) { //check for autoindex if default file is missing
                response.setPath(tempPath);
                response.setAutoIndex(configRoute->m_autoindex);
                return 200;
            }
            else if(ret1 != S_IFREG) {
                PRINT("ERROR, Path is invalid! COuldn't find default file");
                response.setPathFromErrorCode(404);
                return 404;
            }
        }

    if(!(method & configRoute->m_allowedMethods)){ //if you are not allowed to access the resource with GET POST or DELETE
        PRINT("ERROR,you have no rights to access this resource with the Method provided");
        response.setPathFromErrorCode(403);
        return 403;
    }

	/*if cgi*/
	if (mvars.check_extension(configRoute->m_cgi, newfullPath) == true)
	{
		PRINTVAR(newfullPath);
		response.setCgi(true);
		mvars.update_envp(request);
		PRINT(mvars.get_value("REQUEST_METHOD"));
        response.setBody(mvars.cgi_caller(request.getBody()));
        mvars.clean_meta_map();
        return 200;//not always 200
		
	}

    if(method == METH_DELETE){

        // if path is root on delete, send which response?
        deleteResource(newfullPath);
    }
    else if(method == METH_POST){
        uploadResource(newfullPath,request.getBody());
    }
    else {
        response.setPath(newfullPath);
    }
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
