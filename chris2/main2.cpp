#include "RequestParser.hpp"
#include "ResponseBuilder.hpp"

int main() {

    int socket = 1;
    std::string request = "POST /simple.html HTTP/1.1\n\
Host: www.example.com\n\
User-Agent: Mozilla\n\
Accept: text/html\n\
Accept-Language: en-us\n\
Connection: keep-alive\n\
\n\
body is here for POST requests";

    RequestParser parser( request );
    parser.tokenizeRequest();

    ResponseBuilder builder( socket, parser.getHeaderPairs(), parser.getBody() );
    builder.buildResponse();
    builder.printHeaderInfo();
    builder.printBody();

    return (0);
}
