#include "../includes/Request.hpp"

Request::Request() {};

Request::~Request() {};

vector<pair<string, string>> Request::getHeaderPairs( void ) {

    return ( this->_headerPairs );
};

string Request::getBody( void ) {

    return ( this->_body );
};
string Request::getHeaderValueFromKey(const string &key)
{
    for (int i = 0; i < _headerPairs.size(); i++) {
        if(_headerPairs[i].first == key)
            return _headerPairs[i].second;
    }

    return std::string();
}
