#include "Request.hpp"

using namespace std;

Request::Request(std::string& request_str)
{
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
    //set path
    //set host protocol
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

}

Request::~Request(){}

