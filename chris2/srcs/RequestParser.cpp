#include "RequestParser.hpp"

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

vector<pair<string, string>> RequestParser::getHeaderPairs( void ) {

    return ( this->_headerPairs );
};

string RequestParser::getBody( void ) {

    return ( this->_body );
};
