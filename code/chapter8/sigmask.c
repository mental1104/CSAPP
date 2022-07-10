#include <csapp.h>

int main(){
    sigset_t mask, prev_mask;

    Sigemptyset(&mask);
    Sigaddset(&mask, SIGINT);

    /* Block SIGINT and save previous blocked set */
    Sigprocmask(SIG_BLOCK, &mask, &prev_mask);

    Pause();

    /* Restore previous blocked set, unblocking SIGINT */
    Sigprocmask(SIG_SETMASK, &prev_mask, NULL);

    exit(0);
}