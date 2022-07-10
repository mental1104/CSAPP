#include <csapp.h>

int main(){
    pid_t pid;
    pid_t pgrp;
    int status;
    int id;

    pid = getpid();
    pgrp = getpgrp();

    printf("Parent process id=%d\n", pid);
    printf("Parent process group id=%d\n", pgrp);

    id = fork();
    if (id == 0){
        /*
        pid_t tmp_parentid = pid;
        */
        pid = getpid();
        /*
        int tmp = setpgid(tmp_parentid, pid);
        if (tmp == -1){
            printf("error");
        }
        // setpgid(pid, pid);*/
        pgrp = getpgrp();

        printf("Child process id=%d\n", pid);
        printf("Child process group id=%d\n", pgrp);

        exit(0);
    }
    wait(&status);
    pid = getpid();
    pgrp = getpgrp();

    printf("Parent process id=%d\n", pid);
    printf("Parent process group id=%d\n", pgrp);

    exit(0);
}