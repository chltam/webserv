#include "../includes/Response.hpp"
#include "Response.hpp"

Response::Response() {
    _headerFields["Status"] = "NOT SET";
    _headerFields["Date"] = saveDateTime();
    _headerFields["Server"] = "UNKNOWN";
    _headerFields["Content-Type"] = "UNKNOWN";
    _headerFields["Content-Length"] = "UNKNOWN";
    _autoindex = false;
};

Response::~Response() {};

std::string Response::build()
{
    //building body first so i can use the length of it in the content header field
    bodyToString();
    headerToString();

    return (_respHeader + _respBody);
}

void Response::bodyToString()
{
    std::stringstream buffer;
    std::stringstream lenStr;
    std::string bufString;


    if(_autoindex) {
        _respBody = buildIndexPage();
    }
    else{

        std::ifstream file( _path );

        if ( file.is_open() ) {
            buffer << file.rdbuf();
        }
        file.close();
        _respBody = buffer.str();
    }

}


void Response::headerToString()
{
    _headerFields["Content-Length"] = std::to_string(_respBody.length());
    PRINTVAR(_respBody.length());

    _respHeader = ("HTTP/1.1 " + getStringFromStatus(_status) + "\r\nConnection: keep-alive\r\n");

    std::map<std::string,std::string>::const_iterator it = _headerFields.begin();
    for (; it != _headerFields.end(); it++) {

        if(it->first != "Status"){
            _respHeader += it->first + ": " + it->second + "\r\n";
        }
    }
    _respHeader += "\r\n";
}

void Response::setStatus(int status)
{
    _status = status;
}

void Response::setPath(const std::string& path)
{
    _path = path;
}

void Response::setAutoIndex(bool val)
{
    _autoindex = val;
}

const std::string &Response::getResponseBody()
{
    return _respBody;
}

void Response::insertHeaderField(const std::string& key, const std::string& value)
{
    _headerFields[key] = value;
}


void Response::setPathFromErrorCode(int errorCode)
{
    switch (errorCode)
    {
    case 403:
        _path = "./errorpages/error403.html";
        return;
    case 404:
        _path = "./errorpages/error404.html";
        return;
    default:
        _path = "ERROR";
        PRINT("Error, path from error code is incorrect");
        return;
    }
}

std::string Response::buildIndexPage()
{
    //im assuming i get the path to the directory in question
    DIR* directory;
    struct dirent* entry;

    std::string ret("<!doctype html><html><head><title>AUTO INDEX PAGE</title>");

    ret += "<style>.list-text {display: inline-block;width: "; //css code to make it look better
    ret += "150px"; //var to change width
    ret += "; }";
    ret += "em {font-weight: bold;font-style: normal;}"; //making shit bold
    ret += "</style></head><body>";

    ret += "<p >List of files and directories of: <em>";
    ret += _path;
    ret += "</em></p>";
    ret += "<ul>";

    directory = opendir(_path.c_str());
    while ((entry = readdir(directory)))
        ret += createIndexingElement(filetypeToString(entry->d_type),entry->d_name);

    ret += "</ul></body></html>";
    closedir(directory);
    return ret;
}


std::string Response::createIndexingElement(const std::string& fileType,const std::string& relPath)
{
    std::string ret("<li><span class=\"list-text\">");
    ret += fileType;
    ret += ": </span><a href=\"";
    ret +=relPath;
    ret +="\">";
    ret +=relPath;
    ret +="</a></li>";

    return ret;
}

std::string Response::filetypeToString(int type)
{
    std::string ret;

    switch (type)
    {
    case DT_BLK:
        ret = "Block Device";
        break;
    case DT_CHR:
        ret = "Character Device";
        break;
    case DT_DIR:
        ret = "Directory";
        break;
    case DT_FIFO:
        ret = "named Pipe";
        break;
    case DT_LNK:
        ret = "Symbolic Link";
        break;
    case DT_REG:
        ret = "Regular File";
        break;
    case DT_SOCK:
        ret = "UNIX Domain Socket";
        break;
    case DT_UNKNOWN:
        ret = "UNKNOWN";
        break;
    default:
        ret = "ERROR";
        break;
    }

    return ret;
}
