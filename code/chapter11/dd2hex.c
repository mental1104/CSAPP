#include<csapp.h>

int main(int argc, char** argv){
    if(argc!=2){
        fprintf(stderr,"Usage: %s <network byte order>\n", argv[0]);
        exit(0);
    }

    struct in_addr inaddr;
    int rc;
    //char buf[MAXBUF];
    //sscanf(argv[1], "%x", &buf);

    rc = inet_pton(AF_INET, argv[1], &inaddr);

    if(rc == 0)
        app_error("inet_pton error: invalid network byte order\n");
    else if(rc == -1)
        unix_error("inet_pton\n");

    printf("0x%x\n", ntohl(inaddr.s_addr));

    exit(0);
}