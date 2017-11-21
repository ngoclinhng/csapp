#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>

/* External veriables */
extern char **environ;      /* Defined by libc */

/** 
 * Unix-style error
 * @msg the error message
 */
void unix_error(char *msg)
{
    fprintf(stderr, "%s: %s\n", msg, strerror(errno));
    exit(-1);
}

/**
 * Application error
 */
void app_error(char *msg)
{
    fprintf(stderr, "%s\n", msg);
    exit(0);
}

/** 
 * Wrapper for fork
 * Returns a PID on success, otherwise terminates the calling process
 * and reports error
 */
pid_t Fork(void)
{
    pid_t pid;
    if ((pid = fork()) < 0)
        unix_error("Fork error");
    return pid;
}

/**
 * Wrapper for fgets function.
 */
char *Fgets(char *str, int size, FILE *stream)
{
    char *retstr;

    if (((retstr = fgets(str, size, stream)) == NULL) && ferror(stream))
        app_error("Fgets error");

    return retstr;
}


