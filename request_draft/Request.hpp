#ifndef _REQUEST_H_
#define _DEFINE_H_

#include <iostream>
#include <sstream>
#include <cstring>
#include <vector>
#include <map>



class Request{

public:
    Request(std::string& request_str);
    ~Request();
    std::string getHeaderValueFromKey(const std::string &key);
	std::string	get_value(std::string& key);
    std::string getBody();
	std::vector<std::pair<std::string, std::string> >&	get_request_pair();

	void	printf_all();
    
private:
	std::vector<std::pair<std::string, std::string> >	_request_pair;
    std::string _body;

    void    set_path_query(std::string uri);
	void	set_name_port();

};



#endif