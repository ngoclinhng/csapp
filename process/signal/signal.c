/* WARNING: This code is buggy! */

#include "common.h"
#define MAXBUF (8192)      /* Max I/O buffer size */

/* SIGCHILD signal handler */
void handler(int signum)
{
    /* Save errno on entry */
    int olderrno = errno;

    /* reap a child */
    if ((waitpid(-1, NULL, 0)) < 0)
        sio_error("waitpid error");
    Sio_puts("Handler reaped child\n");
    Sleep(1);
    errno = olderrno;
}

int main()
{
   int i, n;
   char buf[MAXBUF];

   /* Install SIGCHILD signal handler */
   Signal(SIGCHLD, handler);

   /* Parent creates children */
   for (i = 0; i < 3; ++i) {
       if (Fork() == 0) {
           printf("Hello from child %d\n", (int)getpid());
           exit(0);
       }
   }

   /* Parent waits for terminal input and process it */
   n = Read(STDIN_FILENO, buf, sizeof(buf));
   printf("Parent processing input\n");
   while (1)
       ;
   
   return 0;
}
