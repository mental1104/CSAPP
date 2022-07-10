#include <csapp.h>

int main(){
    pid_t pid;

    /* Child sleeps until SIGKILL signal received, then dies */
    if((pid = Fork()) == 0) {
        Pause();
        printf("control should never reach here!");
        exit(0);
    }

    /* Parent sends a SIGKILL signal to a child */
    Kill(pid, SIGKILL);
    printf("Child Process has been successfully killed\n");
    exit(0);
}