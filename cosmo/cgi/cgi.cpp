#include <iostream>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <map>


/*
meta_vars
"AUTH_TYPE"
"CONTENT_LENGTH"
"CONTENT_TYPE"
"GATEWAY_INTERFACE"
"PATH_INFO"
"PATH_TRANSLATED"
"QUERY_STRING"
"REMOTE_ADDR"
"REMOTE_HOST"
"REMOTE_IDENT"
"REMOTE_USER"
"REQUEST_METHOD"
"SCRIPT_NAME"
"SERVER_NAME"
"SERVER_PORT"
"SERVER_PROTOCOL"
"SERVER_SOFTWARE"
*/
using namespace std;

std::map<std::string, std::string>	pair_to_map(std::vector<std::pair<std::string, std::string>> vec)
{
	std::map<std::string, std::string> map;

	for(vector<pair<string, string>>::iterator it = vec.begin(); it != vec.end(); it++)
	{
		if(it->first == "request type")
			map.insert(make_pair("REQUEST_METHOD", it->second));
		else if(it->first == "ACCEPT")
			map.insert(make_pair("CONTENT_TYPE", it->second));
		else if(it->first == "path")
			map.insert(make_pair("PATH_INFO", it->second));
		/*SERVER_PROTOCOL=HTTP/1.1*/
		/*SERVER_NAME=127.0.0.1*/
		/*SERVER_PORT=8081*/
	}
	
	return (map);
}

void	set_envp(std::map<std::string, std::string> meta_vars, char ** envp)
{
	/*putenv?*/
}

int	main(int argc, char **argv, char **envp)
{
	std::vector<std::pair<std::string, std::string>> vec {{"key1", "content1"}, {"key2", "content2"}, {"key3", "content3"}};
	std::map<std::string, std::string> map = pair_to_map(vec);
	std::map<std::string, std::string>::iterator it = map.find("key2");
	std::cout << it->second << std::endl;
	it->second = "test";
	std::cout << it->second << std::endl;

	return (0);
}