#include "AResponse.hpp"

#include <cstdio>

class DeleteResponse : public AResponse {

    public:

        DeleteResponse(string path, string header, string reqBody);
        ~DeleteResponse();

    private:

        int exec();
};
