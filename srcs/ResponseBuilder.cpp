#include "../includes/ResponseBuilder.hpp"
#include "ResponseBuilder.hpp"

using namespace std;

ResponseBuilder::ResponseBuilder() {};

ResponseBuilder::~ResponseBuilder() {};

Response* ResponseBuilder::createNewResponse(Request &request, const Config& config, MetaVars& mvars  )
{
    Response* response = new Response();
    response->_headerFields["Server"] = request.getHeaderValueFromKey("Host");

    std::string contType= request.getHeaderValueFromKey("Host");
    std::string::size_type pos = contType.find(',');
    response->_headerFields["Content-Type"] = contType; /*content type*/

    response->setStatus(setResponseStatus(request,config,*response,mvars));

    return response;
}

int ResponseBuilder::setResponseStatus( Request& request, const Config& config, Response& response, MetaVars& mvars )
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

	/*test*/
	
// 	std::size_t start = resourceData.find("boundary=") + 9;
//     std::size_t end = resourceData.find("\r\n", start);
//     std::string boundary = resourceData.substr(start, end - start);

// std::size_t boundaryPos = resourceData.find(boundary);
//     std::size_t headerEnd = resourceData.find("\r\n\r\n", boundaryPos) + 4;

//     std::size_t nextBoundaryPos = resourceData.find(boundary, boundaryPos + boundary.length());
//     while (nextBoundaryPos != std::string::npos) {
//         std::size_t dataStart = headerEnd;
//         std::size_t dataEnd = nextBoundaryPos - 2;
//         std::size_t filenameStart = resourceData.find("filename=\"", headerEnd) + 10;
//         std::size_t filenameEnd = resourceData.find("\"", filenameStart);
//         std::string filename = resourceData.substr(filenameStart, filenameEnd - filenameStart);

//         std::ofstream outputFile(filename, std::ios::binary);
//         if (!outputFile) {
//             std::cerr << "Failed to open file for writing: " << filename << "\n";
//             return;
//         }

//         outputFile.write(resourceData.c_str() + dataStart, dataEnd - dataStart);
//         outputFile.close();

//         headerEnd = resourceData.find("\r\n\r\n", nextBoundaryPos) + 4;
//         nextBoundaryPos = resourceData.find(boundary, nextBoundaryPos + boundary.length());
//     }


	/*test*/

    if ( file.is_open() ) {

        file << resourceData;

    } else {
        cout << "Unable to open file\n";
        // _status = "403 Not Allowed";
    }
    file.close();
}