#include "MetaVars.hpp"
#include "RequestParser.hpp"

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
"SCRIPT_NAME" dont use this
"SCRIPT_FILENAME" use this for php-cgi
"SERVER_NAME"
"SERVER_PORT"
"SERVER_PROTOCOL"
"SERVER_SOFTWARE"
*/
int	main(int argc, char **argv, char **envp)
{
	MetaVars mvars(envp);
    std::string request = "GET /simple.html HTTP/1.1\n"
							"Host: www.example.com\n"
							"User-Agent: Mozilla\n"
							"Accept: text/html\n"
							"Accept-Language: en-us\n"
							"Connection: keep-alive\n"
							"\n"
							"body is here for POST requests\n";

	// mvars.update_envp();
	// mvars.print_envp();

	int pid = fork();

	if (pid == 0)
	{
		mvars.set_value("AUTH_TYPE", "");
		mvars.set_value("CONTENT_LENGTH","");
		mvars.set_value("CONTENT_TYPE", "text/html");
		mvars.set_value("PATH_INFO", "/simple.html");
		mvars.set_value("QUERY_STRING", "");
		mvars.set_value("REQUEST_METHOD", "GET");
		// mvars.set_value("SCRIPT_NAME", "example.php");
		mvars.set_value("SERVER_NAME", "127.0.0.1");
		mvars.set_value("SERVER_PROTOCOL", "HTTP/1.1");
		mvars.set_value("SERVER_PORT", "8081");
		mvars.set_value("SCRIPT_FILENAME", "example.php");
		
		mvars.update_envp();

		execve("/usr/bin/php-cgi", envp, mvars.get_envp());
	}
	else
	{
		int status;
		waitpid(pid, &status, 0);
	}

	return (0);
}