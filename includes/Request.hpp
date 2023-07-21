#ifndef _REQUEST_H_
#define _REQUEST_H_

#include <iostream>
#include <sstream>
#include <cstring>
#include <vector>
#include <map>
#include "webserv.hpp"



class Request{

public:
    Request(std::string request_str);
    ~Request();
    std::string getHeaderValueFromKey(const std::string &key);
    std::string getBody();
	std::vector<std::pair<std::string, std::string> >&	get_request_pair();
    friend std::ostream&operator<<(std::ostream& os,const Request& request);
    bool    getTimeout();

private:
	std::vector<std::pair<std::string, std::string> >	_request_pair;
    std::string _body;

    bool    _timeout;
    void    set_path_query(std::string uri);
	void	set_name_port();

};



#endif
