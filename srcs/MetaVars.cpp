#include "../includes/MetaVars.hpp"
using namespace	std;

MetaVars::MetaVars()
{
	_envp = NULL;
	_envp_size = 0;
}

MetaVars::MetaVars(char** envp)
{
	_envp_size = count_envp_size(envp);
	_envp = copy_envp(envp, _envp_size);
	_envp[_envp_size] = NULL;

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
	set_value("CONTENT_LENGTH", to_string(body.length()));
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

bool	MetaVars::check_extension(const vector<pair<string, string> >& cgi_pair, string path)
{
	int	dpos = path.rfind('.');
	if (dpos == std::string::npos)
		return (false);
	// vector<pair<string , string> >::const_iterator it = cgi_pair.begin();
	// while (it != cgi_pair.end())
	// {
	// 	if (path.substr(dpos) == it->first)
	// 	{
	// 		set_value("SCRIPT_NAME", path);
	// 		set_value("SCRIPT_FILENAME", path);
	// 		set_executor(it->second);
	// 		return (true);
	// 	}
	// 	it++;
	// }
	for (int n = 0; n < cgi_pair.size(); n++)
	{
		PRINTVAR(cgi_pair[n].first);
		PRINTVAR(cgi_pair[n].second);
		PRINTVAR(path);
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

	PRINTVAR(arg[0]);
	PRINTVAR(arg[1]);

	std::string	ret;
	int	pid;
	int fd[2];

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
		write(fd[1], request_body.c_str(), request_body.length());
		close(fd[1]);
		waitpid(pid, NULL, 0);
		char	buffer[100];
		int	bread;
		
		while((bread = read(fd[0], buffer, 100)) != 0)
		{
			buffer[bread] = '\0';
			ret += buffer;
		}
		close(fd[0]);
	}

	PRINTVAR(ret);
	int start = ret.find("<!DOCTYPE html>");
	if (start == std::string::npos)
		return (string());
	ret = ret.substr(start);
	return (ret);

}

void	MetaVars::print_envp()
{
	for (int n = 0; _envp[n]; n++){
		std::cout << _envp[n] << std::endl;
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