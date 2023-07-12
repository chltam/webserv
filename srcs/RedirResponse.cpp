#include "../includes/RedirResponse.hpp"

RedirResponse::RedirResponse(string path,  string header, string reqBody):
    AResponse(path, header, reqBody ) {};

RedirResponse::~RedirResponse() {};

int RedirResponse::exec() {



    return ( EXIT_SUCCESS );
};
