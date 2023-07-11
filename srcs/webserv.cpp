#include "webserv.hpp"

// -1 on invalid path
// else one of these S_IFBLK,S_IFCHR,S_IFDIR,S_IFIFO,S_IFLNK,S_IFREG,S_IFSOCK
// S_IFDIR == directory
// S_IFREG == regular file
int ValidatePath(const std::string &path)
{
    struct stat status;

    int ret = stat(path.c_str(),&status);
    if(ret == -1)
        std::cout << strerror(errno) << std::endl;
    else {
        std::cout << "Success" << std::endl;
        return (status.st_mode & S_IFMT);
    }
}
