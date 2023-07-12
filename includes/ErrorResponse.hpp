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

        ErrorResponse(string path, string header, string reqBody);
        ~ErrorResponse();

    private:

        int exec();
};
