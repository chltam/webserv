#include "../includes/MetaVars.hpp"

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
	// free_envp(_envp);
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

std::string	MetaVars::get_value(std::string key)
{
	std::map<std::string, std::string>::iterator it;

	it = _meta_map.find(key);
	if (it != _meta_map.end())
		return (it->second);
	else
		return ("");
}

void	MetaVars::update_envp()
{
	cout << "size = " <<_meta_map.size() << endl;
	char **new_envp = new char*[_envp_size + _meta_map.size() + 1];
	for (int n = 0; n < _envp_size; n++){
		new_envp[n] = strdup(_envp[n]);
	}
	for (map<string, string>::iterator it = _meta_map.begin(); it != _meta_map.end(); it++){
		new_envp[_envp_size++] = strdup((it->first + "=" + it->second).c_str());
	}
	new_envp[_envp_size] = NULL;
	free_envp(_envp);
	_envp = new_envp;
}

char	**MetaVars::get_envp()
{
	return(_envp);
}

void	MetaVars::print_envp()
{
	for (int n = 0; _envp[n]; n++){
		std::cout << _envp[n] << std::endl;
	}
}

int	count_envp_size(char **envp)
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
char	**copy_envp(char **envp, int& _envp_size)
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

void	free_envp(char **envp)
{
	for (int n = 0; envp[n]; ++n){
		free(envp[n]);
	}
	delete[] envp;
}