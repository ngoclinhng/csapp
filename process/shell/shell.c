#include "common.h"

#define MAXARGS (128)    /* max length of argument list execve */
#define MAXLINE (8192)   /* max text line length */

/**
 * Evaluates command line.
 * Takes as input a null-terminated string (char *) representing
 * command line, forks a child process and executes the appropriate
 * program.
 *
 * @cmdline the input command line.
 */
void eval(char *cmdline);

/**
 * parseline - Parse the command line and build the argv array
 */
int parseline(char *buf, char **argv);

/**
 * builtin_command - If the first argument is a builtin command,
 * run it and return true.
 */
int builtin_command(char **argv);

/**
 * main
 */
int main(void)
{
    char cmdline[MAXLINE];
    
    while (1) { 
        
        printf("> "); 
        Fgets(cmdline, MAXLINE, stdin);

        if (feof(stdin))
            exit(0);

        /* Eveluate */
        eval(cmdline);
    }
    return 0;
}

void eval(char *cmdline)
{
    char *argv[MAXARGS]; /* Argument list execve() */
    char buf[MAXLINE];   /* Holds modified command line */
    int bg;              /* Should the job run in background or foreground? */
    pid_t pid;           /* Process id */

    strcpy(buf, cmdline);
    bg = parseline(buf, argv);

    if (argv[0] == NULL) {
        return; 
    }

    if (!builtin_command(argv)) {
        /* Child run user job */
        if ((pid = Fork()) == 0) {
            if (execve(argv[0], argv, environ) < 0) {
                printf("%s: Command not found.\n", argv[0]);
                exit(0);
            }
        }

        /* Parent waits for foreground job to terminate */
        if (!bg) {
            int status;
            if (waitpid(pid, &status, 0) < 0) {
                unix_error("waitfg: waitpid error");
            }
        }
        else
            printf("%d %s", pid, cmdline);
    }
    return;
}

int builtin_command(char **argv)
{
    if (!strcmp(argv[0], "quit"))
        exit(0);
    if (!strcmp(argv[0], "&"))
        return 1;
    return 0;
}

int parseline(char *buf, char **argv)
{
    char *delim;                    /* Points to the first space delimiter */
    int argc;                       /* Number of arguments */
    int bg;                         /* Background job? */

    buf[strlen(buf) - 1] = ' ';     /* Replace trailing '\n' with space */
    while (*buf && (*buf == ' '))   /* Ignore leading spaces */
        buf++;

    /* Build the argv list */
    argc = 0;
    while ((delim = strchr(buf, ' '))) {
        argv[argc++] = buf;
        *delim = '\0';
        buf = delim + 1;
        while (*buf && (*buf == ' '))
            buf++;
    }
    argv[argc] = NULL;

    if (argc == 0)
        return 1;

    /* Should the job run in the background */
    if ((bg = (*argv[argc-1] == '&')) != 0)
        argv[--argc] = NULL;
    return bg;
}
