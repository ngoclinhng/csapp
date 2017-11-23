#include "common.h"

/* SIGINT handler */
void sigint_handler(int signum)
{
    printf("Caught SIGINT!\n");
    exit(0);
    
}

int main()
{
    /* Install the SIGINT handler */
    Signal(SIGINT, sigint_handler);

    /* Wait for the receipt of a signal */
    Pause();
    printf("Does the control ever reach here?\n");
    exit(0);
}
