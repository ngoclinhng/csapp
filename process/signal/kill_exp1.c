#include "common.h"

int main(void)
{
    pid_t pid;
    
    /* Child sleeps until SIGKILL signal recieved, then dies */
   if ((pid = Fork()) == 0) {
        printf("I am about to sleep until SIGKILL signal arrived\n");
        Pause();
        printf("Control should never reach here\n");
        exit(0);
   }
   
   /* Parent sleeps for 2 seconds */
   Sleep(2);

   /* Parent sends a SIGKILL signal to a child */
   Kill(pid, SIGKILL);

   return 0;
}
