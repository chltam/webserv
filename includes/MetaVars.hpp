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

using namespace std;

class MetaVars{

public:
	MetaVars();
	MetaVars(char** envp);
	~MetaVars();
	// void	insert_pair(std::string key, std::string value);
	void	set_value(std::string key, std::string value);
	int	set_content_length(std::string& body);
	std::string get_value(std::string key);
	char	**get_envp();
	void	update_envp(void);
	void	print_envp(void);

private:
	char**	_envp;
	int		_envp_size;
	std::map<std::string, std::string> _meta_map;

	int	count_envp_size(char **envp);
	char	**copy_envp(char **envp, int& _envp_size);
	void	free_envp(char **envp);

};



#endif