#ifndef __COMMON_H__
#define __COMMON_H__
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>
#include <errno.h>

/******************************************************************************
 * Exrternal variables
 ******************************************************************************/
extern char **environ;              /* Defined by libc */

/******************************************************************************
 * Custom error handlers
 ******************************************************************************/

/**
 * UNIX-style error handler
 * 
 * When a syscall encounters an error, it sets the errno to some appropriate value.
 * This function outputs the @msg along with the information from errno to stdout
 * and then terminates the process.
 *
 * @msg the message to be output
 */
void unix_error(char *msg);

/**
 * Application error.
 *
 * When a user-level function(run in user mode) encounters an error, this function
 * is used to output some helper message to stdout.
 *
 * @msg the message to be output.
 */
void app_error(char *msg);


/******************************************************************************
 * Wrappers for Unix process control functions
 ******************************************************************************/

/**
 * Wrapper for fork.
 *
 * Either success and returns the process identifier of the newly created process
 * or terminates by calling unix_error function if error occurred.
 */
pid_t Fork(void);

/**
 * Wrapper for execve.
 */
void Execve(const char *filename, char *const argv[], char *const envp[]);

/**
 * Wrapper for wait.
 */
pid_t Wait(int *status);

/**
 * Wrapper for waitpid.
 */
pid_t Waitpid(pid_t pid, int *statusp, int options);

/**
 * Wrapper for kill.
 */
void Kill(pid_t pid, int signum);

/**
 * Wrapper for pause.
 */
void Pause(void);

/**
 * Wrapper for sleep.
 */
unsigned int Sleep(int secs);

/**
 * Wrapper for alarm.
 */
unsigned int Alarm(unsigned int secs);

/**
 * Wrapper for setpgid.
 */
void Setpgid(pid_t pid, pid_t pgid);

/**
 * Wrapper for getpgrp.
 */
pid_t Getpgrp(void);


/******************************************************************************
 * Wrappers for Unix signal functions.
 * use: man func_name to figure out corresponding definition.
 ******************************************************************************/
typedef void handler_t(int);
handler_t *Signal(int signum, handler_t *handler);
void Sigprocmask(int how, const sigset_t *set, sigset_t *oldset);
void Sigemptyset(sigset_t *set);
void Sigfillset(sigset_t *set);
void Sigaddset(sigset_t *set, int signum);
void Sigdelset(sigset_t *set, int signum);
int Sigismember(const sigset_t *set, int signum);
int Sigsuspend(const sigset_t *set);


/******************************************************************************
 * Signal-safe I/O routines.
 ******************************************************************************/
ssize_t sio_puts(char s[]);         /* puts string */
ssize_t sio_putl(long v);           /* puts long */
void sio_error(char s[]);           /* puts error message and exits */


/******************************************************************************
 * Sio wrappers.
 ******************************************************************************/
ssize_t Sio_puts(char s[]);
ssize_t Sio_putl(long v);
void Sio_error(char s[]);

/******************************************************************************
 * Wrappers for standard I/O functions.
 ******************************************************************************/
char *Fgets(char *str, int size, FILE *stream);
#endif
