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

        // response->insertHeaderField("Content-Type",contType);


    }
    response->setStatus(setResponseStatus(request,config,*response,mvars));
    response->insertHeaderField("Content-Type", getTypeFromExtension(response->getPath(), config.getTypes()));

    return response;
}

int ResponseBuilder::setResponseStatus( Request& request, const Config& config, Response& response, MetaVars& mvars )
{
    const ConfigServer* server = config.getConfigServerFromRequest( request.getHeaderValueFromKey("Host") );
    if (request.getTimeout())
    {
        response.insertHeaderField("Server", "localhost"); // NEEDS TO BE CHANGED ACCORDING TO PORT ETC.
        response.insertHeaderField("Content-Type", "text/html");
        response.setPath(server->getErrorPageFromCode(408));
        return 408;
    }

    string path = request.getHeaderValueFromKey( "path" );

    PRINTVAR(server);
    PRINTVAR(path);
    const ConfigRoute* configRoute = server->getRouteFromPath( path );


    if(configRoute == NULL){
        PRINT("ERROR, could find CONFIGROUTE, this should never happen!");
    }

//prev: configRoute->m_shouldRedirect == true
    if(configRoute->getRedirectDir().empty() == false) {
        PRINT("REDIRECT SIR!");
        response.insertHeaderField("Location",configRoute->getRedirectDir());
        return 301;
    }

    std::string newfullPath(configRoute->getRoot() + path);
    int ret1 = ValidatePath(newfullPath);
    int method = StringToMethodEnum(request.getHeaderValueFromKey("request type"));
    // PRINTVAR(newfullPath);
    // PRINTVAR(ret1);
    // PRINTVAR(S_IFREG);
    // PRINTVAR(S_IFDIR);
    if(ret1 == -1 && method != METH_POST){
        PRINT("ERROR, Path is invalid!");
        response.setPath(server->getErrorPageFromCode(404));
        return 404;
    }
    else if(ret1 == S_IFDIR && method == METH_GET) { //find,append,validate index file, if not, check for autoindex, else error
        //only check this for a get request
            std::string tempPath(newfullPath);
            if(newfullPath[newfullPath.length()-1] != '/' ){
                newfullPath += "/";
                newfullPath += configRoute->getDefaultFile()[0]; //!!!!! needs additional function
            }
            else
                newfullPath += configRoute->getDefaultFile()[0]; //!!!!! needs additional function
            ret1 = ValidatePath(newfullPath);
            PRINTVAR(ret1);
            if((ret1 == -1 || ret1 == S_IFDIR) && configRoute->getAutoIndex() == true) { //check for autoindex if default file is missing
                response.setPath(tempPath);
                response.setAutoIndex(configRoute->getAutoIndex());
                return 200;
            }
            else if(ret1 != S_IFREG) {
                PRINT("ERROR, Path is invalid! COuldn't find default file");
                response.setPath(server->getErrorPageFromCode(404));
                return 404;
            }
        }

    if(!(method & configRoute->getAllowedMethods())){ //if you are not allowed to access the resource with GET POST or DELETE
        PRINT("ERROR,you have no rights to access this resource with the Method provided");
        response.setPath(server->getErrorPageFromCode(403));
        return 403;
    }

	/*if cgi*/
	if (mvars.check_extension(configRoute->getCgi(), newfullPath) == true)
	{
		// PRINTVAR(newfullPath);
		response.setCgi(true);
		mvars.update_envp(request);
		// PRINT(mvars.get_value("REQUEST_METHOD"));
        response.setBody(mvars.cgi_caller(request.getBody()));
        mvars.clean_meta_map();
        return 200;//not always 200

	}

    if(method == METH_DELETE){

        // if path is root on delete, send which response?
        deleteResource(newfullPath);
    }
    else if(method == METH_POST){
        uploadResource(newfullPath,request.getBody(), request.getHeaderValueFromKey("Content-Type"));
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

void ResponseBuilder::uploadResource(const std::string& newfullPath,std::string resourceData, std::string contentType)
{
    string filename;
    stringstream lenStr;

    cout << "POST received" << endl;
	ofstream	file;
	if (isFileUploading(contentType))
	{
		filename = "upload/" + extractFileName(resourceData);
		resourceData = extractFileData(resourceData);
		file.open(filename.c_str());
	}
	else
	{
		filename = newfullPath;
		file.open(filename.c_str(), std::ios::app);
	}

    if ( file.is_open() ) {

        file << resourceData;

    } else {
        cout << "Unable to open file\n";
        // _status = "403 Not Allowed";
    }
    file.close();
}

bool	ResponseBuilder::isFileUploading(std::string& contenType)
{
	size_t	pos;
	std::string	fileType;

	pos = contenType.find(';');
	if (pos == std::string::npos)
		return (false);
	fileType = contenType.substr(0, pos);
	if (fileType == "multipart/form-data")
		return (true);
	return (false);
}

std::string	ResponseBuilder::extractFileName(std::string& resourceData)
{
	size_t	start;
	size_t	end;
	std::string	filename;

	start = resourceData.find("filename") + 10;
	end = resourceData.find("\"", start);

	filename = resourceData.substr(start, end - start);
	PRINTVAR(filename);
	return (filename);

}

std::string	ResponseBuilder::extractFileData(std::string& resourceData)
{
	std::string	boundary;
	size_t	pos;
	size_t	dataStart;
	size_t	dataEnd;

	pos = resourceData.find("\r\n");
	boundary = resourceData.substr(0, pos);

	dataStart = resourceData.find("\r\n\r\n") + 4;
	dataEnd = resourceData.find(boundary, dataStart) - 2;

	return (resourceData.substr(dataStart, dataEnd - dataStart));

}
