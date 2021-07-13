#include<csapp.h>

int main(int argc, char** argv){
    if(argc!=2){
        fprintf(stderr,"Usage: %s <hex num>\n", argv[0]);
        exit(0);
    }
    struct in_addr inaddr;
    uint32_t addr;
    char buf[MAXBUF];

    sscanf(argv[1], "%x", &addr);
    inaddr.s_addr = htonl(addr);

    if(!inet_ntop(AF_INET, &inaddr, buf, MAXBUF))
        unix_error("inet_ntop\n");
    printf("%s\n", buf);

    exit(0);
}