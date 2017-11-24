#include "common.h"

#define MAXBUF  (8192)      /* Max I/O buffer size */

/* SIGCHLD signal handler */
void handler(int signum)
{
    /* Save errno */
    int olderrno = errno;

    /* Reap children */
    while (waitpid(-1, NULL, 0) > 0) {
        sio_puts("Handler reaped child\n");
    }

    if (errno != ECHILD)
        unix_error("waitpid error");
    Sleep(1);
    errno = olderrno;
}

int main()
{
    int i, n;
    char buf[MAXBUF];
    
    /* Install SIGCHLD signal handler */
    Signal(SIGCHLD, handler);

    /* Parent creates children processes */
    for (i = 0; i < 3; ++i) {
        if (Fork() == 0) {
            printf("Hello from child %d\n", (int)getpid());
            exit(0);
        }
    }

    /* Parent waits for the terminal input and process it */
    n = Read(STDIN_FILENO, buf, sizeof(buf));

    printf("Parent processing input\n");
    while (1)
        ;

    return 0;
        
}
