#include "Server.hpp"

int main(int argc, char **argv, char **envp) {

    (void)argc;
    

	Server	tserver(argv[1], envp);
	tserver.set_server_sock(); //should that be part of the object creation?
	cout << "here\n" << endl;
	tserver.start_listening();
	tserver.accept_connection();

    return (0);
}
