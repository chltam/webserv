#include "MetaVars.hpp"

MetaVars::MetaVars(){};

MetaVars::~MetaVars(){};

MetaVars::MetaVars(MetaVars& ref)
{

}

MetaVars&	MetaVars::operator=(MetaVars& ref)
{

}

// void	MetaVars::insert_pair(std::string key, std::string value)
// {
// 	_meta_map.insert(std::make_pair(key, value));
// }

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