#include "Config.hpp"

int main(int ac, char** av)
{
    Config conf(av[1]);

    std::cout << conf << std::endl;
    PRINT("THE END");
    return 0;
}