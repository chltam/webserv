#include "RequestParser.hpp"
#include "ResponseBuilder.hpp"

int main() {

    int socket = 1;
    string request = "POST /simple.html HTTP/1.1\r\n\
Host: www.example.com\r\n\
User-Agent: Mozilla\r\n\
Accept: text/html\r\n\
Accept-Language: en-us\r\n\
Connection: keep-alive\r\n\
\r\n\
body is here for POST requests";

    RequestParser parser( request );
    parser.tokenizeRequest();

    ResponseBuilder builder( socket, parser.getHeaderPairs(), parser.getBody() );
    builder.buildResponse();
    builder.printHeaderInfo();
    builder.printBody();

    return (0);
}
