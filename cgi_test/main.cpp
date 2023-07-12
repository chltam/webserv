#include "MetaVars.hpp"

int main(int argc, char **argv, char **envp)
{
	/*
	export AUTH_TYPE=""
	export CONTENT_LENGTH=""
	export CONTENT_TYPE="text/html"
	export PATH_INFO=""
	export QUERY_STRING=""
	export REQUEST_METHOD="GET"
	export SCRIPT_FILENAME="./cgi-bin/index.php"
	export SERVER_NAME="127.0.0.1"
	export SERVER_PROTOCOL="HTTP/1.1"
	export SERVER_PORT="8081"
	export REDIRECT_STATUS=0 
	*/
	
	MetaVars	mvars(envp);

	mvars.set_value("CONTENT_TYPE", "text_html");
	mvars.set_value("CONTENT_LENGTH", "");
	mvars.set_value("PATH_INFO", "");
	mvars.set_value("QUERY_STRING", "param=1&param=2");
	mvars.set_value("REQUEST_METHOD", "GET");
	mvars.set_value("SCRIPT_FILENAME", "index.php");
	mvars.set_value("SCRIPT_NAME", "index.php");
	mvars.set_value("SERVER_NAME", "127.0.0.1");
	mvars.set_value("SERVER_PROTOCOL", "HTTP/1.1");
	mvars.set_value("SERVER_PORT", "8080");
	mvars.set_value("REDIRECT_STATUS", "0");
	mvars.update_envp();
	// mvars.print_envp();

	// std::string	cgi = "/usr/bin/python3";
	std::string	cgi = "/usr/bin/php-cgi";
	// std::string	cgi = "./php-cgi8.2";

	char *arg[3];
	arg[0] = (char *)cgi.c_str();
	arg[1] = (char *)mvars.get_value("SCRIPT_NAME").c_str();
	arg[2] = NULL;
	std::cout << arg[0] << std::endl;
	std::cout << arg[1] << std::endl;
	int	fd[2];
	// int backup = dup(STDOUT_FILENO);
	pipe(fd);
	int pid = fork();
	if (pid == 0)
	{
		dup2(fd[1], 1);
		close(fd[0]);
		// close(fd[1]);
		// execve(arg[0], arg, mvars.get_envp());
		execve(arg[0], arg, mvars.get_envp());
		// std::cout << "here" << std::endl;
	}
	else
	{
		waitpid(pid, NULL, 0);
		char buffer[500];
		int bread = 1;
		std::string	ret;
		// close(fd[1]);
		while (bread != 0)
		{
			std::cout << "1123" << std::endl;

			read(fd[0], buffer, 500);
			std::cout << "1" << std::endl;
			ret += buffer;
		}
		// close(fd[0]);
		// buffer[bread] = '\0';
		std::cout << ret << std::endl;
	}

	std::string str = "Hello\0World";



}