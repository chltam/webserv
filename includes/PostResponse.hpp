#include "AResponse.hpp"

class PostResponse : public AResponse {

    public:

        PostResponse(string path, string header, string reqBody);
        ~PostResponse();

    private:

        int exec();
};
