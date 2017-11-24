#include "common.h"

#define MAXLINE (8192)  /* Max length of command line string */
#define MAXARGS (128)   /* Max length of argument list execve */

/**
 * parse_cmdline - parses the command line and build the argv list.
 *
 * @buf - the command line.
 * @argv - the argv list to be built.
 *
 * @return true(1) if the job to be run is a background job, i.e
 * the last character in the @buf is ampersand &, otherwise
 * return false(0) indicating that the job is foreground job.
 */
int parse_cmdline(char *buf, char *argv[]);

/**
 * builtin_command - If the first argument is a builtin command,
 * run it and return true.
 */
int builtin_command(char *argv[]);

/**
 * eval - Evaluates command line.
 *
 * @cmdline - the user's input command line.
 */
void eval(char *cmdline);

/**
 * sigchld_handler - SIGCHLD signal handler.
 */
void sigchld_handler(int signum);


int main()
{
    char cmdline[MAXLINE];
    
    /* Install SIGCHLD signal handler */
    Signal(SIGCHLD, sigchld_handler);

    while (1) {
        printf("unix_shell> ");
        fflush(stdout);

        /* Read user input */
        Fgets(cmdline, MAXLINE, stdin);

        if (feof(stdin))    exit(0);

        /* Evaluate user's input */
        eval(cmdline);
    }
    return 0;
}

/* parse_cmdline */
int parse_cmdline(char *buf, char *argv[])
{
    int bg;         /* background job? */
    int argc;       /* number of arguments in arguments list argv */
    char *delim;    /* first space in buf */

    /* replace the last newline with space */
    buf[strlen(buf)-1] = ' ';

    /* skip leading spaces */
    while (*buf && (*buf == ' '))   buf++;

    argc = 0;
    while ((delim = strchr(buf, ' ')) != NULL) {
        argv[argc++] = buf;
        *delim = '\0';
        buf = delim + 1;

        /* skip until first non-whitespace character */
        while (*buf && (*buf == ' '))   buf++;
    }        
   
    /* append NULL to argv */
    argv[argc] = NULL;

    if (argc == 0)  return 1;
    
    if ((bg = (*argv[argc - 1] == '&')) != 0) argv[--argc] = NULL;
    return bg;
}

int builtin_command(char *argv[])
{
    if (!strcmp(argv[0], "quit"))   exit(0);
    if (!strcmp(argv[0], "&"))      return 1;
    return 0;
}

void eval(char *cmdline)
{
    char *argv[MAXARGS];
    char buf[MAXLINE];
    int bg;
    pid_t pid;

    sigset_t mask, prev;

    Sigemptyset(&mask);
    Sigaddset(&mask, SIGCHLD);

    strcpy(buf, cmdline);
    bg = parse_cmdline(buf, argv);
    if (argv[0] == NULL) return; 
    if (!builtin_command(argv)) {

        /* Child run user's job */
        if ((pid = Fork()) == 0) {
            if (execve(argv[0], argv, environ) < 0) {
                printf("%s: Command not found.\n", argv[0]);
                exit(0);
            }
        }

        /* Parent waits for foreground job to terminate */
        if (!bg) {
            /* temporarily block SIGCHLD */
            Sigprocmask(SIG_BLOCK, &mask, &prev); 
            Waitpid(pid, NULL, 0);
            /* Unblock SIGCHLD */
            Sigprocmask(SIG_SETMASK, &prev, NULL);
        }
        else {
            printf("%d %s", pid, cmdline);
        }
    }
    return;
}

void sigchld_handler(int signum)
{
    int olderrno = errno;
    
    while (waitpid(-1, NULL, 0) > 0) {
        sio_puts("reaped a child.");
    }

    if (errno != ECHILD)    unix_error("Waitpid error^_^");
    errno = olderrno;
}

