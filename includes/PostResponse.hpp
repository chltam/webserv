#include "AResponse.hpp"

class PostResponse : public AResponse {

    public:

        PostResponse( string path, string serverName, string contType, string reqBody );
        ~PostResponse();

    private:

        int exec();
};
