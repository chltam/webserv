#include "../includes/Request.hpp"
#include "Request.hpp"


using namespace std;

Request::Request(std::string request_str)
{
    if ( request_str.empty() ) {
        _timeout = true;
        return ;
    }
    else
        _timeout = false;

	PRINTVAR(request_str);

    std::istringstream   str_iss(request_str);
    vector<string>    lines;
    string getline_buffer;

    while (getline(str_iss, getline_buffer, '\n'))
        lines.push_back(getline_buffer);

    vector<string>::iterator it = lines.begin();
    istringstream   line_iss(*it);
    vector<string>  tokens;

    // handle first request line
    while (getline(line_iss, getline_buffer, ' '))
        tokens.push_back(getline_buffer);
    _request_pair.push_back(make_pair("request type", tokens[0]));
    set_path_query(tokens[1]);//split the query string after '?'
    _request_pair.push_back(make_pair("protocol", tokens[2]));
    tokens.clear();
    it++;

    // handle remaining lines
    while (it != lines.end())
    {
        // after the header, save the body
        if ( *it == "\r" ) {

            ++it;  // skip the empty line
            while (it != lines.end()) {
                _body += *it + '\n';
                ++it;
            }
            break; // we are done
        }
        // save the header info
        size_t split_position = it->find(": ");
        if (split_position != string::npos) {
            string key = it->substr(0, split_position);
            string value = it->substr(split_position + 2); // +2 to skip over ': '
            _request_pair.push_back(make_pair(key, value));
        }
        tokens.clear();
        it++;
    }
	set_name_port(); //read the 'Host' pair and split into 'server_name' and 'server_port'

}

Request::~Request(){}

std::string Request::getHeaderValueFromKey(const std::string &key)
{
    for (int i = 0; i < _request_pair.size(); i++) {
        if(_request_pair[i].first == key)
            return _request_pair[i].second;
    }
    return std::string();
}

std::string	Request::getBody()
{
	return (_body);
}

std::vector<std::pair<std::string, std::string> >& Request::get_request_pair()
{
	return (_request_pair);
}

void    Request::set_path_query(string uri)
{
    int split = uri.find('?');
    if (split != string::npos)
    {
        _request_pair.push_back(make_pair("path", uri.substr(0, split)));
        _request_pair.push_back(make_pair("query", uri.substr(split + 1)));
    }
    else
        _request_pair.push_back(make_pair("path", uri));
}

void	Request::set_name_port()
{
	for (int n = 0; n < _request_pair.size(); n++)
	{
		if (_request_pair[n].first == "Host")
		{
			int split = _request_pair[n].second.find(':');
			if (split != string::npos)
			{
				_request_pair.push_back(make_pair("server_name", _request_pair[n].second.substr(0, split)));
				_request_pair.push_back(make_pair("server_port", _request_pair[n].second.substr(split + 1)));
			}
			else
			{
				_request_pair.push_back(make_pair("server_name", _request_pair[n].second));
				_request_pair.push_back(make_pair("server_port", "80"));
			}
		}
	}

	// vector<pair<string, string> >::iterator	it = _request_pair.begin();

	// while (it != _request_pair.end())
	// {
	// 	if (it->first == "Host")
	// 	{
	// 		int split = it->second.find(':');
	// 		if (split != string::npos)
	// 		{
	// 			_request_pair.push_back(make_pair("server_name", it->second.substr(0, split)));
	// 			_request_pair.push_back(make_pair("server_port", it->second.substr(split + 1)));
	// 		}
	// 		else
	// 		{
	// 			_request_pair.push_back(make_pair("server_name", it->second));
	// 			_request_pair.push_back(make_pair("server_port", "80"));
	// 		}
	// 	}
	// 	it++;
	// }
}

bool Request::getTimeout()
{
    return _timeout;
}

std::ostream& operator<<(std::ostream& os,const Request& request)
{
  	for (int n = 0; n < request._request_pair.size(); n++)
	{
		os <<" [" << request._request_pair[n].first << "] = [" <<  request._request_pair[n].second <<"]" << endl;
	}

	os << request._body;

    return os;
}
