#include "MetaVars.hpp"

int	main(int argc, char **argv, char **envp)
{
	MetaVars mvars(envp);

	mvars.set_value("test0", "0");
	mvars.set_value("test1", "1");
	mvars.set_value("test2", "2");
	mvars.set_value("test3", "3");
	mvars.print_envp();

	return (0);
}