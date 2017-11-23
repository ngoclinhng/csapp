#include "common.h"

/**
 * Parent creates N processes and waits for all of its children to terminate
 * in order they are created.
 */
#define N (2)

int main(void)
{
    pid_t pids[N];
    pid_t retpid;
    int i, status;

    /* Parents create N processes */
    for (i = 0; i < N; ++i) {
        if ((pids[i] = Fork()) == 0) /* Child */
            exit(i + 100);
    }

    /* Parent reaps its children in order they are created */
    i = 0;
    while ((retpid = waitpid(pids[i++], &status, 0)) > 0) {
        if (WIFEXITED(status)){
            printf("The process %d terminated normally with exit status %d\n",
                    retpid, WEXITSTATUS(status));
        }
        else {
            printf("The process %d terminated abnormally\n", retpid);
        }
    }

    /*
     * If everything is working correctly, the wait set should be empty
     * in the last iteration of the above while loop, i.e there are no more
     * children to reap.
     * So, the last call to waitpid will set errno to ECHILD indicating that
     * there are no more children to reap.
     */
    if (errno != ECHILD)
        unix_error("waitpid error");

    return 0;
}
