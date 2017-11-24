#include "common.h"

volatile sig_atomic_t pid;

/* SIGCHLD signal handler */
void sigchld_handler(int signum)
{
    int olderrno = errno;
    pid = Waitpid(-1, NULL, 0);
    errno = olderrno;
}

/* SIGINT signal handler */
void sigint_handler(int signum)
{
    sio_puts("Caught SIGINT!\n");
    _exit(0);
}

int main()
{
    sigset_t mask, prev;

    /* Install SIGCHLD & SIGINT signals handlers */
    Signal(SIGCHLD, sigchld_handler);
    Signal(SIGINT, sigint_handler);
    Sigemptyset(&mask);
    Sigaddset(&mask, SIGCHLD);

    while (1) {
        /* Block SIGCHLD before forking */
        Sigprocmask(SIG_BLOCK, &mask, &prev);

        /* Fork a child */
        if (Fork() == 0) /* Child */
            exit(0);
        
        pid = 0;
        Sigprocmask(SIG_SETMASK, &prev, NULL);      /* Unblock SIGCHLD */

        /* Parent waits for SIGCHLD to be received (wasteful) */
        while (!pid)
            ;

        /* Do some work after receiving SIGCHLD */
        printf(".");
    }

    return 0;
}
