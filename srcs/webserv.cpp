#include "webserv.hpp"

// -1 on invalid path
// else one of these S_IFBLK,S_IFCHR,S_IFDIR,S_IFIFO,S_IFLNK,S_IFREG,S_IFSOCK
// S_IFDIR == directory
// S_IFREG == regular file
int ValidatePath(const std::string &path)
{
    struct stat status;

    int ret = stat(path.c_str(),&status);
    if(ret == -1){
        PRINT_WARNING(strerror(errno));
        return ret;
    }
    return (status.st_mode & S_IFMT);
}

int getFileSize(const std::string &path)
{
    struct stat status;

    int ret = stat(path.c_str(),&status);
    if(ret == -1){
        PRINT_WARNING(strerror(errno));
        return ret;
    }
    PRINT_LOG("Successfully getting file size");
    return (status.st_size);
}


std::string MethodEnumToString(int val)
{
    std::string ret;
    if(val == METH_NONE)
        return "NONE";
    if(val & METH_GET)
        ret += "GET ";
    if(val & METH_POST)
        ret += "POST ";
    if(val & METH_DELETE)
        ret += "DELETE ";
    if(val > METH_ALL)
        ret+= "INVALID METHODS";
    return ret;
}

int StringToMethodEnum(const std::string &method)
{
    if(method == "GET")
        return METH_GET;
    if(method == "POST")
        return METH_POST;
    if(method == "DELETE")
        return METH_DELETE;
    return METH_NONE;
}

std::string saveDateTime() {

    time_t rawtime;
    tm* timeinfo;
    char buffer[ 80 ];

    time( &rawtime );
    //  All HTTP date/time stamps MUST be represented in Greenwich Mean Time (GMT)!!!
    timeinfo = gmtime( &rawtime );

    strftime( buffer, sizeof(buffer), "%a, %d %b %Y %T %Z", timeinfo );
    std::string str( buffer );
    return str;
};

std::string getFileContent( const std::string& filename ) {

    std::stringstream buffer;
    std::string bufString;

    std::ifstream file( filename.c_str() );

    if ( file.is_open() ) {
        buffer << file.rdbuf();
    }
    file.close();
    return ( buffer.str() );
}


std::string getStringFromStatus(int status)
{
    switch (status)
    {
    case 200:
        return "200 OK";
        break;
    case 301:
        return "301 Moved Permanently";
        break;
    case 400:
        return "400 Bad Request";
        break;
	case 401:
        return "401 Unauthorized";
        break;
    case 403:
        return "403 Forbidden";
        break;
	case 404:
        return "404 Not Found";
        break;
	case 405:
        return "405 Method Not Allowed";
        break;
	case 413:
        return "413 Content Too Large";
        break;
    case 408:
        return "408 Timeout";
        break;
	case 500:
		return "500 Internal Server Error";
		break;
    default:
        return "INVALID";
        break;
    }

    return "ERROR";
}

std::string getTypeFromExtension( std::string path,const std::map<std::string, std::string>& types) {


    std::string ext = path.substr(path.rfind('.') + 1);

    std::map<std::string, std::string>::const_iterator it = types.find(ext);
    if (it == types.end())
        return "text/html";

    return it->second;

}

long	getUnixTime()
{
	struct timeval tv;

	gettimeofday(&tv, NULL);
	return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

std::string	toString(int number)
{
	std::stringstream ss;
    ss << number;
    return ss.str();
}
