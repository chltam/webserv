#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>


#define MAXLINE 1000

int main(int ac, char **av) {

    struct addrinfo *p, *listp, hints;
    char buf[MAXLINE];
    int rc, flags;

    if(ac != 2) {
        fprintf(stderr,"usage: %s <domain name>\n",av[0]);
        exit(0);
    }

    memset(&hints,0,sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    if((rc = getaddrinfo(av[1],NULL,&hints,&listp))!= 0){
        fprintf(stderr,"getaddrinfo error: %s\n",gai_strerror(rc));
        exit(1);
    }

    flags = NI_NUMERICHOST;
    for (p = listp; p; p = p->ai_next) {
        getnameinfo(p->ai_addr,p->ai_addrlen,buf,MAXLINE,NULL,0,flags);
        printf("%s\n",buf);
    }


    return (0);
}
