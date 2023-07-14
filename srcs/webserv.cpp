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
        std::cout << strerror(errno) << std::endl;
        return ret;
    }
    std::cout << "Success" << std::endl;
    return (status.st_mode & S_IFMT);
}

int getFileSize(const std::string &path)
{
    struct stat status;

    int ret = stat(path.c_str(),&status);
    if(ret == -1){
        std::cout << strerror(errno) << std::endl;
        return ret;
    }
    std::cout << "Successfully getting file size" << std::endl;
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

    std::ifstream file( filename );

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
    case -200:
        return "200 OK";
        break;
    case 404:
        return "404 Not Found";
        break;   
    case 408:
        return "408 Timeout";
        break;   
    case 403:
        return "403 Forbidden";
        break;
    case 301:
        return "301 Moved Permanently";
        break; 
    default:
        return "INVALID";
        break;
    }

    return "ERROR";
}