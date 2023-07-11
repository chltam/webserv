#include <sys/stat.h>
#include <iostream>
#include <fstream>
#include <string>
#include <errno.h>
#include <string.h>

int main(int ac, char** av) {

    struct stat status;

    int ret = stat(av[1],&status);
    if(ret == -1)
        std::cout << strerror(errno) << std::endl;
    else{
        std::cout << "Success" << std::endl;
        switch (status.st_mode & S_IFMT) {
           case S_IFBLK:  printf("block device\n");            break;
           case S_IFCHR:  printf("character device\n");        break;
           case S_IFDIR:  printf("directory\n");               break;
           case S_IFIFO:  printf("FIFO/pipe\n");               break;
           case S_IFLNK:  printf("symlink\n");                 break;
           case S_IFREG:  printf("regular file\n");            break;
           case S_IFSOCK: printf("socket\n");                  break;
           default:       printf("unknown?\n");                break;
           }

           printf("I-node number:            %ju\n", (uintmax_t) status.st_ino);

           printf("Mode:                     %jo (octal)\n",
                  (uintmax_t) status.st_mode);

           printf("Link count:               %ju\n", (uintmax_t) status.st_nlink);
           printf("Ownership:                UID=%ju   GID=%ju\n",
                  (uintmax_t) status.st_uid, (uintmax_t) status.st_gid);

           printf("Preferred I/O block size: %jd bytes\n",
                  (intmax_t) status.st_blksize);
           printf("File size:                %jd bytes\n",
                  (intmax_t) status.st_size);
           printf("Blocks allocated:         %jd\n",
                  (intmax_t) status.st_blocks);

           printf("Last status change:       %s", ctime(&status.st_ctime));
           printf("Last file access:         %s", ctime(&status.st_atime));
           printf("Last file modification:   %s", ctime(&status.st_mtime));

           exit(EXIT_SUCCESS);
    }
    return 0;
}
