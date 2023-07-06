#ifndef _METAVARS_H_
#define _METAVARS_H_

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <map>

class MetaVars{

public:
	MetaVars();
	MetaVars(char** envp);
	MetaVars(MetaVars& ref);
	MetaVars&	operator=(MetaVars& ref); 
	~MetaVars();
	// void	insert_pair(std::string key, std::string value);
	void	set_value(std::string key, std::string value);
	std::string get_value(std::string key);
	void	update_envp(void);
	void	print_envp(void);
private:
	char**	_envp;
	int		_envp_size;
	std::map<std::string, std::string> _meta_map;

};


#endif