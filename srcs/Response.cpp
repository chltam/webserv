#include "../includes/Response.hpp"

Response::Response() {
    _headerFields["Status"] = "NOT SET";
    _headerFields["Date"] = saveDateTime();
    _headerFields["Server"] = "UNKNOWN";
    _headerFields["Content-Type"] = "UNKNOWN";
    _headerFields["Content-Length"] = "UNKNOWN";
	_cgi = false;
};

Response::~Response() {};

std::string Response::build()
{
    headerToString();
    bodyToString();

    return (_respHeader + _respBody);
}

void Response::bodyToString()
{
    std::stringstream buffer;
    std::stringstream lenStr;
    std::string bufString;


    std::ifstream file( _path );

    if (_cgi == true)
        return ;

    if ( file.is_open() ) {
        buffer << file.rdbuf();
    }
    file.close();

    _respBody = buffer.str();
}


void Response::headerToString()
{
    std::string size;
    if (_cgi == true)
        size = std::to_string(_respBody.size());
    else
        size = std::to_string(getFileSize(_path));
    if(size == "-1")
        _headerFields["Content-Length"] = "0";
    else
        _headerFields["Content-Length"] = size;


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

void    Response::setBody(const std::string& body)
{
    _respBody = body;
}

void	Response::setCgi(bool state)
{
	_cgi = state;
}

bool	Response::getCgi()
{
	return (_cgi);
}

const std::string& Response::getResponseBody()
{
    return _respBody;
}