#include "Server.hpp"

int main(int argc, char **argv, char **envp) {

    (void)argc;
    (void)argv;
    // configuration = readfile(file);
    // Server server = Server( AF_INET, SOCK_STREAM, 0, 8080, INADDR_ANY, 10 );
    // server.startListening();

	Server	tserver(argv[1]);
	tserver.set_server_sock(); //should that be part of the object creation?
	tserver.start_listening();
	tserver.accept_connection();

    return (0);
}
