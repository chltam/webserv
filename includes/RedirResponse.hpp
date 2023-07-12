#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <ctime>

#include <unistd.h>

#include "AResponse.hpp"

class RedirResponse : public AResponse {

    public:

        RedirResponse(string path, string header, string reqBody);
        ~RedirResponse();

    private:

        int exec();
};
