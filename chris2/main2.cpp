#include "RequestParser.hpp"
#include "ResponseBuilder.hpp"

int main() {

    std::string request = "GET /index.html HTTP/1.1\n\
Host: www.example.com\n\
User-Agent: Mozilla\n\
Accept: text/html\n\
Accept-Language: en-us\n\
Connection: keep-alive\n\
\n\
body is here for POST requests";

    RequestParser parser( request );
    parser.tokenizeRequest();

    ResponseBuilder builder( parser.getHeaderPairs(), parser.getBody() );

    return (0);
}
