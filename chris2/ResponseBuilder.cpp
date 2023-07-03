#include "ResponseBuilder.hpp"

ResponseBuilder::ResponseBuilder( std::vector<std::pair<std::string, std::string>> headerPairs,
    std::string body ) {

    _requestHeaderPairs = headerPairs;
    _requestBody = body;
};
