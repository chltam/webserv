#include "Config.hpp"

int main(int ac, char **av) {

    if(ac != 2)
        return 1;
    Config c(av[1]);
    c.printServers();
    

    return (0);
}
