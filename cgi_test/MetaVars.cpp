#include "MetaVars.hpp"
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

void	MetaVars::set_content_length(std::string& body)
{
	set_value("CONTENT_LENGTH", to_string(body.length()));
}

void	MetaVars::set_executor(std::string executor)
{
	_executor = executor;
}

// void	MetaVars::update_envp() //need rework, update from request and more
// {
// 	cout << "size = " <<_meta_map.size() << endl;
// 	char **new_envp = new char*[_envp_size + _meta_map.size() + 1];
// 	for (int n = 0; n < _envp_size; n++){
// 		new_envp[n] = strdup(_envp[n]);
// 	}
// 	for (map<string, string>::iterator it = _meta_map.begin(); it != _meta_map.end(); it++){
// 		new_envp[_envp_size++] = strdup((it->first + "=" + it->second).c_str());
// 	}
// 	new_envp[_envp_size] = NULL;
// 	free_envp(_envp);
// 	_envp = new_envp;
// }

std::string	MetaVars::get_value(std::string key)
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

std::string	MetaVars::cgi_caller()
{
	char	*arg[3];
	arg[0] = (char *)_executor.c_str();
	arg[1] = (char *)get_value("SCRIPT_NAME").c_str();
	arg[2] = NULL;


	std::cout << arg[0] << std::endl;
	std::cout << arg[1] << std::endl;
	// std::cout << arg[2] << std::endl;
	std::string request_body = "HEY THIS IS THE REQUEST BODY\n";
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
		// dup2(fd[0], 0);
		dup2(fd[1], 1);
		close(fd[0]);
		close(fd[1]);
		execve(arg[0], arg, get_envp());
			//if failed do sth
	}
	else
	{
		// write(fd[1], request_body.c_str(), request_body.length());
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

	// _envp_size = 0;
	// while (envp[_envp_size])
	// 	_envp_size++;
	
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