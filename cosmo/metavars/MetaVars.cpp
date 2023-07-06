#include "MetaVars.hpp"

MetaVars::MetaVars()
{
	_envp = NULL;
	_envp_size = 0;
}

MetaVars::MetaVars(char** envp)
{
	_envp_size = 0;
	while (envp[_envp_size])
		_envp_size++;
	_envp = new char*[_envp_size + 1];
	for (int n = 0; n < _envp_size; n++){
			_envp[n] = strdup(envp[n]);
	}
	_envp[_envp_size] = NULL;
}

MetaVars::~MetaVars()
{
	for (int n = 0; _envp[n]; ++n){
		free(_envp[n]);
	}
	delete[] _envp;
}

MetaVars::MetaVars(MetaVars& ref)
{

}

MetaVars&	MetaVars::operator=(MetaVars& ref)
{

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
	
}

void	MetaVars::print_envp()
{
	for (int n = 0; _envp[n]; n++){
		std::cout << _envp[n] << std::endl;
	}
	for (std::map<std::string, std::string>::iterator it = _meta_map.begin(); it != _meta_map.end(); it++){
		std::cout << it->first << " = " << it->second << std::endl;
	}

}