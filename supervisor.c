/***************************************************************************
 * Author: eddix<elitecharm@gmail.com>
 *
 * All rights reserved
 **************************************************************************/

#include "supervisor.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

char* Usage = "supervisor - supervise your service\nUsage:\n\tsupervise {service command}\n";

int main(int argc, char **argv)
{
    pid_t childpid;
    int status, fd;

    if (argc < 2) {
        puts(Usage);
        exit(EXIT_SUCCESS);
    }

    // fork once to create daemon supervisor progress.
    childpid = fork();
    if (-1 == childpid) {
        perror("fork()");
        exit(EXIT_FAILURE);
    } else if (0 != childpid) {
        exit(EXIT_SUCCESS);
    }

    // redirect stdin, stdout, stderr to /dev/null
    fd = open("/dev/null", O_RDWR);
    dup2(fd, STDIN);
    dup2(fd, STDOUT);
    dup2(fd, STDERR);
    if (fd > 2) {
        close(fd);
    }

    // fork twice to run application
    while (1) {
        childpid = fork();
        if (-1 == childpid) {
            //perror("fork()");
            exit(EXIT_FAILURE);
        } else if (0 != childpid) {
            waitpid(childpid, &status, 0);
            //printf("Server process exited with status %d\n", status);
            sleep(1);
        } else {
            execvp(argv[1], &argv[1]);
            exit(EXIT_FAILURE);
        }
    }
}

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
