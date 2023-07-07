#include "RequestParser.hpp"

RequestParser::RequestParser( std::string sockBuffer ) {

    _buffer = sockBuffer;
};

void RequestParser::tokenizeRequest( void ) {

    // split whole request into lines 
    std::istringstream line_iss( _buffer ); 
    std::vector<std::string> lines;
    std::string tmp_line;

    while ( std::getline( line_iss, tmp_line, '\n' )) {
        lines.push_back( tmp_line );
    }

    // get key-value pairs from lines
    std::vector<std::string>::const_iterator it = lines.begin();
    std::istringstream token_iss( *it );
    it++;
    std::vector<std::string> tokens;
    std::string tmp_token;    

    // handle first request line
    while ( std::getline( token_iss, tmp_token, ' ' )) {
        tokens.push_back( tmp_token );
    }
    _headerPairs.push_back(std::make_pair("REQUEST_METHOD", tokens[0]));
    _headerPairs.push_back(std::make_pair("PATH_INFO", tokens[1]));
    _headerPairs.push_back(std::make_pair("SERVER_PROTOCOL", tokens[2]));
	
	_meta_vars.insert(std::make_pair("REQUEST_METHOD", tokens[0]));
	_meta_vars.insert(std::make_pair("PATH_INFO", tokens[1]));
	_meta_vars.insert(std::make_pair("SERVER_PROTOCOL", tokens[2]));
    
	tokens.clear();

    // handle remaining lines
    while ( it != lines.end() ) {

        // after the header, save the body
        if (it->empty()) {
            ++it;  // skip the empty line
            while (it != lines.end()) {
                _body += *it + '\n';
                ++it;
            }
            break; // we are done
        }
        // save the header info
        size_t split_position = it->find(": ");
        if (split_position != std::string::npos) {
            std::string key = it->substr(0, split_position);
            std::string value = it->substr(split_position + 2); // +2 to skip over ': '
            _headerPairs.push_back(std::make_pair(key, value));
        }
        tokens.clear();
        it++;
    }

    this->printHeaderPairs();
    this->printBody();
}

void RequestParser::printHeaderPairs( void ) {

    for (std::vector<std::pair<std::string, std::string> >::const_iterator it = _headerPairs.begin(); it != _headerPairs.end(); ++it) {
        std::cout << "Key: " << it->first << ", Value: " << it->second << std::endl;
    }
	std::string mvars[] = {"REQUEST_METHOD", "PATH_INFO", "SERVER_PROTOCOL"};
	for(std::map<std::string, std::string>::iterator it = _meta_vars.begin(); it != _meta_vars.end(); ++it)
	{
		std::cout << it->first << " = " << it->second << std::endl;
	}
	
	
}

void RequestParser::printBody( void ) {

    std::cout << "BODY: " << this->_body << std::endl;
}

std::vector<std::pair<std::string, std::string>> RequestParser::getHeaderPairs( void ) {

    return ( this->_headerPairs );
};

std::string RequestParser::getBody( void ) {

    return ( this->_body );
};
