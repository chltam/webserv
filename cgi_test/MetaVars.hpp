#ifndef _METAVARS_H_
#define _METAVARS_H_

#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <map>


class MetaVars{

public:
	MetaVars();
	MetaVars(char** envp);
	~MetaVars();
	// void	insert_pair(std::string key, std::string value);
	void	set_value(std::string key, std::string value);
	void	set_content_length(std::string& body);
	void	set_executor(std::string executor);
	void	update_envp(void);
	std::string get_value(std::string key);
	char	**get_envp();
	std::string	cgi_caller();
	void	print_envp(void);
	void	clean_meta_map();

private:
	char**	_envp;
	int		_envp_size;
	std::string	_executor;
	std::map<std::string, std::string> _meta_map;

	int	count_envp_size(char **envp);
	char	**copy_envp(char **envp, int& _envp_size);
	void	free_envp(char **envp);


};



#endif