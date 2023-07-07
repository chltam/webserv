#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <ctime>

#include <unistd.h>

#include "AResponse.hpp"

class GetResponse : public AResponse {

    public:

        GetResponse( string path, string serverName, string contType, string reqBody );
        ~GetResponse();

    private:

        int exec();
};
