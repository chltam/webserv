#include "AResponse.hpp"

#include <cstdio>

class DeleteResponse : public AResponse {

    public:

        DeleteResponse( string path, string serverName, string contType, string reqBody );
        ~DeleteResponse();

    private:

        int exec();
};
