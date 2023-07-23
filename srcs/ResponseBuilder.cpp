#include "../includes/ResponseBuilder.hpp"
#include "ResponseBuilder.hpp"

using namespace std;

ResponseBuilder::ResponseBuilder() {};

ResponseBuilder::~ResponseBuilder() {};

Response* ResponseBuilder::createNewResponse(Request &request, const Config& config, MetaVars& mvars, Socket& socket )
{
    Response* response = new Response();

    if (request.getTimeout() || request.get_badRequest())
    {
        response->setStatus(setResponseStatus(request,config,*response,mvars,socket));
        response->insertHeaderField("Content-Type", getTypeFromExtension(response->getPath(), config.getTypes()));
        return response;
    }
    response->insertHeaderField("Server",request.getHeaderValueFromKey("Host"));
    std::string contType= request.getHeaderValueFromKey("Accept");
    response->setStatus(setResponseStatus(request,config,*response,mvars,socket));
    response->insertHeaderField("Content-Type", getTypeFromExtension(response->getPath(), config.getTypes()));

    return response;
}

int ResponseBuilder::setResponseStatus( Request& request, const Config& config, Response& response, MetaVars& mvars, Socket& socket )
{
    const ConfigServer* server = config.getConfigServerFromRequest( socket.get_host() );

    if (request.getTimeout() )
    {
        response.insertHeaderField("Server", "localhost"); // NEEDS TO BE CHANGED ACCORDING TO PORT ETC.
        response.insertHeaderField("Content-Type", "text/html");
        response.setPath(config.m_errorPages.find(408)->second);
        return 408;
    }

    if (request.get_badRequest())
    {
        response.setPath(config.m_errorPages.find(400)->second);
        return 400;
    }

	if (server == NULL)
	{
		response.insertHeaderField("Server", "localhost"); // NEEDS TO BE CHANGED ACCORDING TO PORT ETC.
        response.insertHeaderField("Content-Type", "text/html");
        response.setPath(config.m_errorPages.find(408)->second);
        return 408;
	}

    string path = request.getHeaderValueFromKey( "path" );

    const ConfigRoute* configRoute = server->getRouteFromPath( path );
    PRINT_LOG("Config ROute from path:",path,"-----\n",*configRoute);

    if(configRoute == NULL){
        PRINT_ERROR("ERROR, could find CONFIGROUTE, this should never happen!,path = ",path);
    }

    if(configRoute->getRedirectDir().empty() == false) {
        PRINT_LOG("Redirecting to:",configRoute->getRedirectDir());
        response.insertHeaderField("Location",configRoute->getRedirectDir());
        return 301;
    }

    std::string newfullPath(configRoute->getRoot() + path);
    int fileInfo = ValidatePath(newfullPath);
    int method = StringToMethodEnum(request.getHeaderValueFromKey("request type"));
    if(fileInfo == -1 && method != METH_POST){
        PRINT_ERROR("ERROR, Path is invalid!, PATH:",newfullPath);
        response.setPath(server->getErrorPageFromCode(404));
        return 404;
    }
    else if(fileInfo == S_IFDIR && method == METH_GET) { //find,append,validate index file, if not, check for autoindex, else error
        //only check this for a get request
            std::string tempPath(newfullPath);
            if(newfullPath[newfullPath.length()-1] != '/' ){
                newfullPath += "/";
                fileInfo = configRoute->findValidIndexFile(newfullPath);
            }
            else
                fileInfo = configRoute->findValidIndexFile(newfullPath);
            if((fileInfo == -1 || fileInfo == S_IFDIR) && configRoute->getAutoIndex() == true) { //check for autoindex if default file is missing
                response.setPath(tempPath);
                response.setAutoIndex(configRoute->getAutoIndex());
                return 200;
            }
            else if(fileInfo != S_IFREG) {
                PRINT_LOG("Path is invalid! Couldn't find a single valid default file, default files:");
                const std::vector<std::string>& indexFiles = configRoute->getDefaultFile();
                for (std::vector<std::string>::const_iterator it = indexFiles.begin(); it != indexFiles.end(); it++)
                    PRINT_LOG(*it);
                response.setPath(server->getErrorPageFromCode(404));
                return 404;
            }
        }

	if (method & configRoute->getAllowedMethods() && socket.get_permsIssue())
	{
        PRINT_LOG("Method allowed but not authorized");
        response.setPath(server->getErrorPageFromCode(401));
		return 401;	
	}

	if (!(method & configRoute->getAllowedMethods()) && socket.get_needAuthorized() && !socket.get_permsIssue())
	{
        PRINT_LOG("Authorized but method not allowed");
        response.setPath(server->getErrorPageFromCode(403));
		return 403;
	}

    if(!(method & configRoute->getAllowedMethods())){ //if you are not allowed to access the resource with GET POST or DELETE
        PRINT_LOG("ERROR,you have no rights to access this resource with the Method provided");
        PRINT_LOG("Method used,",MethodEnumToString(method),"Methods allowed:",MethodEnumToString(configRoute->getAllowedMethods()));
        response.setPath(server->getErrorPageFromCode(405));
        return 405;
    }

    if(socket.get_sizeIssue())
    {
        PRINT("request body too big");
        response.setPath(server->getErrorPageFromCode(413));
        return 413;
    }

	/*if cgi*/
	if (mvars.check_extension(configRoute->getCgi(), newfullPath) == true)
	{
		// PRINTVAR(newfullPath);
		response.setCgi(true);
		mvars.update_envp(request);
		// PRINT(mvars.get_value("REQUEST_METHOD"));
        response.setBody(mvars.cgi_caller(request.getBody()));
		if (mvars.get_cgi_failure())
		{
			response.setPath(server->getErrorPageFromCode(500));
			response.setCgi(false);
            mvars.clean_meta_map();
       		return 500;
		}
        mvars.clean_meta_map();
        return 200;

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
		filename = UPLOAD_DIR + extractFileName(resourceData);
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
	PRINT_LOG("filename =",filename);
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
