/* $begin restart */
#include "csapp.h"

sigjmp_buf buf;

void handler(int sig) 
{
    siglongjmp(buf, 1);
}

int main() 
{   
    Signal(SIGINT, handler);
    sleep(5);
    if (!sigsetjmp(buf, 1)) {        
	    Sio_puts("starting\n");
    }
    else 
	Sio_puts("restarting\n");

    while(1) {
	Sleep(1);
	Sio_puts("processing...\n");
    }
    exit(0); /* Control never reaches here */
}
/* $end restart */