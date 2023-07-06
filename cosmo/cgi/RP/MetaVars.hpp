#ifndef _METAVARS_H_
#define _METAVARS_H_

#include <iostream>
#include <vector>
#include <map>

class MetaVars{

public:
	MetaVars();
	MetaVars(MetaVars& ref);
	MetaVars&	operator=(MetaVars& ref); 
	~MetaVars();
	// void	insert_pair(std::string key, std::string value);
	void	set_value(std::string key, std::string value);
	std::string get_value(std::string key);

private:
	std::map<std::string, std::string> _meta_map;

};


#endif