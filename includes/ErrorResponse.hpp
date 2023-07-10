#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <ctime>

#include <unistd.h>

#include "AResponse.hpp"

class ErrorResponse : public AResponse {

    public:

        ErrorResponse( string path, string serverName, string contType, string reqBody, string status );
        ~ErrorResponse();

    private:

        int exec();
};
