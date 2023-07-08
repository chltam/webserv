#include "RequestParser.hpp"


// GET / HTTP/1.1
// Host: 127.0.0.1:8080
// Connection: keep-alive
// sec-ch-ua: "Chromium";v="112", "Brave";v="112", "Not:A-Brand";v="99"
// sec-ch-ua-mobile: ?0
// sec-ch-ua-platform: "Linux"
// Upgrade-Insecure-Requests: 1
// User-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/112.0.0.0 Safari/537.36
// Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8
// Sec-GPC: 1
// Accept-Language: en-US,en
// Sec-Fetch-Site: none
// Sec-Fetch-Mode: navigate
// Sec-Fetch-User: ?1
// Sec-Fetch-Dest: document
// Accept-Encoding: gzip, deflate, br


RequestParser::RequestParser( string sockBuffer ) {

    _buffer = sockBuffer;
};

void RequestParser::tokenizeRequest( void ) {

    cout << _buffer << endl;

    // split whole request into lines
    istringstream line_iss( _buffer );
    vector<string> lines;
    string tmp_line;

    while ( getline( line_iss, tmp_line, '\n' )) {
        lines.push_back( tmp_line );
    }

    // get key-value pairs from lines
    vector<string>::const_iterator it = lines.begin();
    istringstream token_iss( *it );
    it++;
    vector<string> tokens;
    string tmp_token;

    // handle first request line
    while ( getline( token_iss, tmp_token, ' ' )) {
        tokens.push_back( tmp_token );
    }
    _headerPairs.push_back(make_pair("request type", tokens[0]));
    _headerPairs.push_back(make_pair("path", tokens[1]));
    _headerPairs.push_back(make_pair("protocol", tokens[2]));
    tokens.clear();

    // handle remaining lines
    while ( it != lines.end() ) {

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
            _headerPairs.push_back(make_pair(key, value));
        }
        tokens.clear();
        it++;
    }

    this->printHeaderPairs();
    this->printBody();
}

void RequestParser::printHeaderPairs( void ) {

    for (vector<pair<string, string> >::const_iterator it = _headerPairs.begin(); it != _headerPairs.end(); ++it) {
        cout << "Key: " << it->first << ", Value: " << it->second << endl;
    }
}

void RequestParser::printBody( void ) {

    cout << "BODY: " << this->_body << endl;
}

vector<pair<string, string> > RequestParser::getHeaderPairs( void ) {

    return ( this->_headerPairs );
};

string RequestParser::getBody( void ) {

    return ( this->_body );
};

std::string RequestParser::getHeaderValueFromKey(const std::string &key)
{
    for (int i = 0; i < _headerPairs.size(); i++) {
        if(_headerPairs[i].first == key)
            return _headerPairs[i].second;
    }

    return std::string();
}
