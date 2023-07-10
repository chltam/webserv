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
    std::string getBody();
    
private:
    std::vector<std::pair<std::string, std::string> >    _request_pair;
    std::string _body;

    void    set_path_query(std::string uri);

};



#endif