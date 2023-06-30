#include "server.hpp"

int main(int ac, char **av) {

    (void)ac;
    (void)av;

    Server server = Server( "127.0.0.1", 8080 );
    server.startListening();

    return (0);
}
