/**
 * common.c - common routines for Unix programming.
 */
#include "common.h"

/*****************************************************************************************
 * Custom error handlers.
 * ***************************************************************************************/
void unix_error(char *msg)
{
    fprintf(stderr, "%s: %s\n", msg, strerror(errno));
    exit(0);
}

void app_error(char *msg)
{
    fprintf(stderr, "%s\n", msg);
    exit(0);
}


/*****************************************************************************************
 * Wrappers for Unix process control functions.
 * ***************************************************************************************/
pid_t Fork(void)
{
    pid_t pid;

    if ((pid = fork()) < 0)
        unix_error("Fork error");
    return pid;
}

void Execve(const char *filename, char *const argv[], char *const envp[])
{
    if (execve(filename, argv, envp) < 0)
        unix_error("Execve error");
}

pid_t Wait(int *status)
{
    pid_t retpid;

    if ((retpid = wait(status)) < 0)
        unix_error("Wait error");
    return retpid;
}

pid_t Waitpid(pid_t pid, int *statusp, int options)
{
    pid_t retpid;

    if ((retpid = waitpid(pid, statusp, options)) < 0)
        unix_error("Waitpid error");
    return retpid;
}

void Kill(pid_t pid, int signum)
{
    int ret;

    if ((ret = kill(pid, signum)) < 0)
        unix_error("Kill error");
}

void Pause(void)
{
    pause();
    return;
}

unsigned int Sleep(int secs)
{
    int ret;

    if ((ret = sleep(secs)) < 0)
        unix_error("Sleep error");
    return ret;
}

unsigned int Alarm(unsigned int secs)
{
    return alarm(secs);
}

void Setpgid(pid_t pid, pid_t pgid)
{
    int ret;

    if ((ret = setpgid(pid, pgid)) < 0)
        unix_error("Setpgrp error");
    return;
}

pid_t Getpgrp(void)
{
    return getpgrp();
}


/*****************************************************************************************
 * Wrappers for Unix signal functions.
 * ***************************************************************************************/

handler_t *Signal(int signum, handler_t *handler)
{
    struct sigaction action, old_action;

    action.sa_handler = handler;
    sigemptyset(&action.sa_mask);
    action.sa_flags = SA_RESTART;

    if (sigaction(signum, &action, &old_action) < 0)
        unix_error("Signal error");
    return old_action.sa_handler;
}

void Sigprocmask(int how, const sigset_t *set, sigset_t *oldset)
{
    if (sigprocmask(how, set, oldset) < 0)
        unix_error("Sigprocmask error");
}

void Sigemptyset(sigset_t *set)
{
    if (sigemptyset(set) < 0)
        unix_error("Sigemptyset error");
}

void Sigfillset(sigset_t *set)
{
    if (sigemptyset(set) < 0)
        unix_error("Sigemptyset error");
}

void Sigaddset(sigset_t *set, int signum)
{
    if (sigaddset(set, signum) < 0)
        unix_error("Sigaddset error");
}

void Sigdelset(sigset_t *set, int signum)
{
    if (sigdelset(set, signum) < 0)
        unix_error("Sigdelset error");
}

int Sigismember(const sigset_t *set, int signum)
{
    return sigismember(set, signum);
}

int Sigsuspend(const sigset_t *set)
{
    int ret = sigsuspend(set);
    if (errno != EINTR)
        unix_error("Sigsuspend error");
    return ret;
}


/*****************************************************************************************
 * Signal safe I/O routines
 * ***************************************************************************************/

/* private sio functions */
/**
 * sio_reverse - Reverse a null-terminated string.
 */
static void sio_reverse(char s[])
{
   int i, j, len;
   char c;

   len = strlen(s);

   for (i = 0, j = len - 1; i < j; ++i, --j) {
       c = s[i];
       s[i] = s[j];
       s[j] = c;
   }
}

/**
 * sio_ltoa - Covert a long to base b string
 */
static void sio_ltoa(long v, char s[], int b)
{
    int c, i = 0;
    int neg = v < 0;

    if (neg)    v = -v;

    do {
        s[i++] = ((c = (v % b)) < 10) ? c + '0' : c - 10 + 'a';
    } while ((v /= b) > 0);

    if (neg)    s[i++] = '-';

    s[i] = '\0';
    sio_reverse(s);
}

/**
 * sio_strlen - Returns the length of a null-terminated string.
 */
static size_t sio_strlen(char s[])
{
    int len = 0;
    
    while (s[len] != '\0')
        ++len;
    return len;
}

/* Public Sio functions */
/**
 * Async-signal-safely outputs  a string to stdout.
 */
ssize_t sio_puts(char s[])
{
    return write(STDOUT_FILENO, s, sio_strlen(s));
}

/**
 * sio_putl - Output a long to stdout
 */
ssize_t sio_putl(long v)
{
    char s[128];

    sio_ltoa(v, s, 10);
    return sio_puts(s);
}

/**
 * sio_error - Output error message and exit.
 */
void sio_error(char s[])
{
    sio_puts(s);
    _exit(1);
}

/*****************************************************************************************
 * WrappWrappers for the SIO routines
 * ***************************************************************************************/
ssize_t Sio_puts(char s[])
{
    ssize_t n;
    if ((n = sio_puts(s)) < 0)
        unix_error("Sio_puts error");
    return n;
}

ssize_t Sio_putl(long v)
{
    ssize_t n;
    if ((n = sio_putl(v)) < 0)
        unix_error("Sio_putl error");
    return n;
}

void Sio_error(char s[])
{
    sio_error(s);
}

/*****************************************************************************************
 * Wrappers for standard I/O functions.
 * ***************************************************************************************/
/**
 * Fgets - reads at most one less than the number of characters specified by @size
 * from the give @stream and stores them in the string @str.
 * Reading stops when a newline character is found, at end-of-file or error.
 * The newline, if any is retained. If any characters are read and there
 * is no error, a '\0' character is appended to end the string.
 *
 * @return unpon successful completion fgets returns a pointer to the read string
 */
char *Fgets(char *str, int size, FILE *stream)
{
    char *retstr;
    if (((retstr = fgets(str, size, stream)) == NULL) && ferror(stream))
        app_error("Fgets error");
    return retstr;
}

/*****************************************************************************************
 * Wrappers for Unix I/O routines.
 * ***************************************************************************************/
ssize_t Read(int fd, void *buf, size_t nbyte)
{
    ssize_t ret;
    if ((ret = read(fd, buf, nbyte)) < 0)
        unix_error("Read error");
    return ret;
}

/*****************************************************************************************
 * Wrappers for dynamic storage allocation functions.
 * ***************************************************************************************/
void *Malloc(size_t size)
{
    void *p;
    if ((p = malloc(size)) == NULL)
        unix_error("Malloc error");
    return p;
}

void *Realloc(void *ptr, size_t size)
{
    void *p;
    if ((p = realloc(ptr, size)) == NULL)
        unix_error("Realloc error");
    return p;
}

void *Calloc(size_t nmemb, size_t size)
{
    void *p;
    if ((p = calloc(nmemb, size)) == NULL)
        unix_error("Calloc error");
    return p;
}

void *Free(void *ptr)
{
    free(ptr);
}
