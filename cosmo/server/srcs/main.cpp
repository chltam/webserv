#include "Server.hpp"

int main(int ac, char **av) {

    (void)ac;
    (void)av;
    // configuration = readfile(file);
    // Server server = Server( AF_INET, SOCK_STREAM, 0, 8080, INADDR_ANY, 10 );
    // server.startListening();

	Server	tserver;
	tserver.set_server_sock();
	cout << "here\n" << endl;
	tserver.start_listening();
	tserver.accept_connection();

    return (0);
}
