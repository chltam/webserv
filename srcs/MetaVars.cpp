#include "../includes/MetaVars.hpp"
#include "../includes/webserv.hpp"
using namespace	std;

MetaVars::MetaVars()
{
	_envp = NULL;
	_envp_size = 0;
	_cgi_fail = false;

}

MetaVars::MetaVars(char** envp)
{
	_envp_size = count_envp_size(envp);
	_envp = copy_envp(envp, _envp_size);
	_envp[_envp_size] = NULL;
	_cgi_fail = false;

}

MetaVars::~MetaVars()
{
	free_envp(_envp);
}


void	MetaVars::set_value(std::string key, std::string value)
{
	std::map<std::string, std::string>::iterator it;

	it = _meta_map.find(key);
	if (it == _meta_map.end())
		_meta_map.insert(std::make_pair(key, value));
	else
		it->second = value;
}

void	MetaVars::set_content_length(std::string body)
{
	set_value("CONTENT_LENGTH", toString(body.length()));
}

void	MetaVars::set_executor(std::string executor)
{
	_executor = executor;
}

/**
 * @brief get info from Request, set MetaVars accordingly
*/
void	MetaVars::update_envp(Request& request)
{
	//read request, set value needed

	set_value("REQUEST_METHOD", request.getHeaderValueFromKey("request type"));
	// set_value("SCRIPT_FILENAME", request.getHeaderValueFromKey("path"));
	// set_value("SCRIPT_NAME", request.getHeaderValueFromKey("path"));


	set_value("SERVER_PROTOCOL", request.getHeaderValueFromKey("protocol"));
	set_value("SERVER_NAME", request.getHeaderValueFromKey("server_name"));
	set_value("SERVER_PORT", request.getHeaderValueFromKey("server_port"));

	set_content_length(request.getBody());

	set_value("PATH_INFO", request.getHeaderValueFromKey("path")); //depends on our approach
	set_value("QUERY_STRING", request.getHeaderValueFromKey("query"));

	set_value("REDIRECT_STATUS", "200");
}

std::string MetaVars::get_value(std::string key)
{
	std::map<std::string, std::string>::iterator it;

	it = _meta_map.find(key);
	if (it != _meta_map.end())
		return (it->second);
	else
		return ("");
}

/**
 * @brief create a new envp by combining original one and _meta_map
*/
char	**MetaVars::get_envp()
{
	char	**new_envp = new char*[_envp_size + _meta_map.size() + 1];

	for (int n = 0; n < _envp_size; n++){
		new_envp[n] = strdup(_envp[n]);
	}

	int	n = 0;
	for (map<string, string>::iterator it = _meta_map.begin(); it != _meta_map.end(); it++){
		new_envp[_envp_size  + n] = strdup((it->first + "=" + it->second).c_str());
		n++;
	}
	new_envp[_envp_size + _meta_map.size()] = NULL;

	return(new_envp);
}

bool	MetaVars::get_cgi_failure(){
	return (_cgi_fail);
}

bool	MetaVars::check_extension(const vector<pair<string, string> >& cgi_pair, string path)
{
	size_t	dpos = path.rfind('.');
	if (dpos == std::string::npos)
		return (false);
	// vector<pair<string , string> >::const_iterator it = cgi_pair.begin(); //commented out by Jean as its unused (Werror)
	for (size_t n = 0; n < cgi_pair.size(); n++)
	{
		if (path.substr(dpos) == cgi_pair[n].first)
		{
			set_value("SCRIPT_NAME", path);
			set_value("SCRIPT_FILENAME", path);
			set_executor(cgi_pair[n].second);
			return (true);
		}
	}
	return (false);
}

std::string	MetaVars::cgi_caller(std::string request_body)
{
	char	*arg[3];
	std::string script = get_value("SCRIPT_NAME");
	arg[0] = (char* )_executor.c_str();
	arg[1] = (char* )script.c_str();
	arg[2] = NULL;

	std::string	ret;
	int	pid;
	int fd[2];

	_cgi_fail = false;
	if (pipe(fd) == -1)
		exit(EXIT_FAILURE); // dc from client?

	pid = fork();
	if (pid == -1)
		exit(EXIT_FAILURE); // dc from client?

	if (pid == 0)
	{
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
		execve(arg[0], arg, get_envp());
			PRINT("HERERERE\n");
	}
	else
	{
		char	buffer[100];
		int	bread = 1;
		int status;

		write(fd[1], request_body.c_str(), request_body.length());
		PRINT("HERE1");

		long start = getUnixTime();
		while (1)
		{
			if (getUnixTime() - start > 200)
				break ;
		}
		waitpid(pid, &status, -1);
		PRINT("HERE2");
		if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
		{
			PRINT(WEXITSTATUS(status));
			_cgi_fail = true;
		}

		pollfd pfd = {fd[0], POLLIN, 0};
		while (1)
		{
			int result = poll(&pfd, 1, 100);
			PRINTVAR(pfd.revents);
			if (getUnixTime() - start > 500)
			{
				_cgi_fail = true;
				kill(pid, SIGINT);
				break;
			}
			if (result <= 0)
				break ;
			else if (pfd.revents & POLLIN)
			{
				bread = read(fd[0], buffer, 100);
				PRINTVAR(bread);
				if (bread <= 0){
					break ;
				}
				else{
					buffer[bread] = '\0';
					ret += buffer;
				}
			}
		}
		if(ret.empty())
			_cgi_fail = true;
		// while(bread != 0)
		// {
		// 	if (getUnixTime() - start > 500)
		// 	{
		// 		PRINT("HERE3");
		// 		_cgi_fail = true;
		// 		kill(pid, SIGINT);
		// 		break;
		// 	}
		// 	PRINT("before read");

		// 	bread = read(fd[0], buffer, 100);
		// 	buffer[bread] = '\0';
		// 	ret += buffer;
		// 	PRINTVAR(ret);

		// }

		close(fd[1]);
		close(fd[0]);
	}
	PRINTVAR(ret);
	size_t start = ret.find("<!DOCTYPE html>");
	if (start == std::string::npos)
	{
		_cgi_fail = true;
		return (string());
	}
	ret = ret.substr(start);
	return (ret);

}

void	MetaVars::print_envp()
{
	for (int n = 0; _envp[n]; n++){
		PRINT(_envp[n]);
	}
}

int	MetaVars::count_envp_size(char **envp)
{
	int	_envp_size = 0;

	while (envp[_envp_size])
		_envp_size++;
	return (_envp_size);
}

/**
 *  @brief Create a copy of the envp array
 *
 *	@param envp original envp
 *	@param _envp_size size of the envp
 *	@return new copy of envp
*/
char	**MetaVars::copy_envp(char **envp, int& _envp_size)
{
	char **new_envp;

	new_envp = new char*[_envp_size + 1];
	for (int n = 0; n < _envp_size; n++){
			new_envp[n] = strdup(envp[n]);
	}
	new_envp[_envp_size] = NULL;
	return (new_envp);
}

void	MetaVars::clean_meta_map()
{
	_meta_map.clear();
	_cgi_fail = false;
}

void	MetaVars::free_envp(char **envp)
{
	if (!envp)
		return ;
	for (int n = 0; envp[n]; ++n){
		free(envp[n]);
	}
	delete[] envp;
}
