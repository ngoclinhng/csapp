#include "common.h"

/* Hey compiler, donot cache this global data on register. Whenever it
 * is referenced in the code, go to the memory and fetch it
 */
volatile long counter = 2;

/* SIGUSR1 signal handler */
void handler(int signum)
{
    int olderrno = errno;
    
    /* Temperarily block all signals before accessing global data */
    sigset_t mask, prev_mask;
    Sigfillset(&mask);
    Sigprocmask(SIG_BLOCK, &mask, &prev_mask);

    /* Access global data */
    Sio_putl(--counter);

    /* Retore mask */
    Sigprocmask(SIG_SETMASK, &prev_mask, NULL);

    /* Restore errno */
    errno = olderrno;

    _exit(0);
}

int main()
{
    pid_t pid;
    sigset_t mask, prev_mask;

    printf("%ld", counter);
    fflush(stdout);

    /* Install SIGUSR1 signal handler */
    Signal(SIGUSR1, handler);

    /* Parent creates one child process that's supposed to run infinitely */
    if ((pid = Fork()) == 0) {
        while (1)
            ;
    }

    /* Parent sends SIGUSR1 to child */
    Kill(pid, SIGUSR1);

    /* Parent reaps child */
    Waitpid(-1, NULL, 0);

    /* Temperarily block all signals */
    Sigfillset(&mask);
    Sigprocmask(SIG_BLOCK, &mask, &prev_mask);
    printf("%ld", ++counter);
    Sigprocmask(SIG_SETMASK, &prev_mask, NULL);

    return 0;
}
