#include "Request.hpp"

int main()
{
	std::string str = "GET /example.html?test=123&st=456 HTTP/1.1\r\n"
"Host: www.example.com:8080\r\n"
"User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/99.0.9999.99 Safari/537.36\r\n"
"Accept: text/html,application/xhtml+xml\r\n"
"Accept-Language: en-US,en;q=0.5\r\n"
"Connection: keep-alive\r\n\r\n"
"qqqqqqqqqqqqqqqqqqqqqqqqq123\r\n";

	Request req(str);

	req.printf_all();



}