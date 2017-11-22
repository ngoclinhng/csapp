#ifndef __PROCESS_H__
#define __PROCESS_H__
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
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

/**
 * Wrapper for sleep function.
 * Suspends the execution of the the calling process until the number
 * of seconds specified has elapsed or a signal recieved.
 *
 * @secs the number of seconds to sleep
 * @return the remaining seconds in the case that sleep has interupted by
 * some signal so it hasnot completed.
 */
unsigned int Sleep(unsigned int secs)
{
    unsigned int ret;
    ret = sleep(secs);
    return ret;
}

/**
 * Wrapper for pause
 * Suspend the execution of the calling process until it recieves some
 * signal to either continue or terminate itself.
 */
void Pause()
{
    (void)pause();
    return;
}

/**
 * Wrapper for kill function.
 *
 * Sends a signal to a process
 *
 * @pid the process identifier. If pid is equal to zero, then
 * Kill sends signal to every process in the process group of
 * the calling process, including the calling process itself.
 * If pid is less than zero, then Kill sends signal to every
 * process in the process group |pid|. Finally, if pid is 
 * greater than zero, Kill sends signal to the process that has 
 * process id of pid
 *
 * @sig the positive integer identifier of the signal to be sent
 * On MAC OSX system we can use man signal to figure it out.
 *
 * @return 0 if OK, -1 if error.
 */
void Kill(pid_t pid, int sig)
{
    int ret;

    if ((ret = kill(pid, sig)) < 0)
        unix_error("Kill error");
}

/*********************************************************************************
 * Wrappers for Unix signal functions
 * *******************************************************************************/

/* signal handler type */
typedef void handler_t(int);

/**
 * Installs the @handler for signal @signum
 * Changes the action associated with the signal @signum (except SIGKILL and SIGSTOP
 * whose default actions cannot be changed) in one of three ways:
 * - If @handler is SIG_IGN, then signals of type @signum are ignored.
 * - If @handler is SIG_DFL, then the action for signals of type
 *   @signum reverts to the default action.
 * - Otherwise, @handler is the address of a user-defined function, called a signal
 *   handler, that will be called whenever the process receives a signal of type
 *   @signum
 *
 * @signum the signal number, see man signal for more details
 * @handler either SIG_IGN, SIG_DFL or user-defined handler.
 *
 * @return a pointer to a previous handler if OK, SIG_ERR on error.
 */
handler_t *Signal(int signum, handler_t *handler)
{
    struct sigaction, old_action;

    action.sa_handler = handler;
    sigemptyset(&action.sa_mask); /* Block sigs of type being handled */
    action.sa_flags = SA_RESTART; /* Restart syscalls if possible */

    if (sigaction(signum, &action, &old_action) < 0)
        unix_error("Signal error");
    return old_action.sa_handler;
}

/**
 * Changes the set of currently blocked signals in the blocked bit vector
 * The specific behaviors depends on the value of @how:
 * - SIG_BLOCK. Add the signals in the @set to blocked, i.e blocked = blocked | @set
 * - SIG_UNBLOCK. Remove the signals in the @set from blocked, i,e
 *   blocked = blocked & ~@set.
 * - SIG_SETMASK. blocked = @set.
 * If @oldset is not null, the previous value of blocked bit vector is stored
 * in @oldset
 */
void Sigprocmask(int how, const sigset_t *set, sigset_t *oldset)
{
    if (sigprocmask(how, set, oldset) < 0)
        unix_error("Sigprocmask error");
    return;
}

/**
 * Initializes @set to the empty set
 */
void Sigemptyset(sigset_t *set)
{
    if (sigemptyset(set) < 0)
        unix_error("Sigemptyset error");
    return;
}

/**
 * Adds all possible signals to @set
 */
void Sigfillset(sigset_t *set)
{
    if (sigfillset(set) < 0)
        unix_error("Sigfillset error");
    return;
}

/**
 * Adds signal @signum to @set
 */
void Sigaddset(sigset_t *set, int signum)
{
    if (sigaddset(set, signum) < 0)
        unix_error("Sigaddset error");
    return;
}

/**
 * Removes @signum from @set
 */
void Sigdelset(sigset_t *set, int signum)
{
    if (sigdelset(set, signum) < 0)
        unix_error("Sigdelset error");
    return;
}

/**
 * Is @signum a member of @set?
 */
int Sigismember(const sigset_t *set, int signum)
{
    int ret;

    if ((ret = sigismember(set, signum)) < 0)
        unix_error("Sigismember error");
    return ret;
}

/**
 * Temporarily changes blocked bit vector to blocked = blocked | mask, i.e
 * adds the signals in @mask to blocked and then suspends the process until
 * the receipt of a signal whose action is either to run a handler or to
 * terminate the process.
 * If the action is to terminate, then the process terminates without returning 
 * from sigsuspend.
 * If the action is to run a handler, the sigsuspend returns after the handler
 * returns, restoring the blocked set to its state when sigsuspend was called.
 *
 * The sigsuspend function is equivalent to an atomic (uninteruptable) version
 * of the following:
 *  sigprocmask(SIG_BLOCK, &mask, &prev);
 *  pause();
 *  sigprocmask(SIG_SETMASK, &prev, NULL)
 *
 *  @return allways returns -1
 */
int Sigsuspend(const sigset_t *mask)
{
    int ret = sigsuspend(mask);
    if (errno != EINTR)
        unix_error("Sigsuspend error");
    return ret;
}

#endif
