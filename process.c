#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
    /*pid_t origin = getpid();*/
    /*printf("origin = %d\n", origin);*/
    /*pid_t pid = fork();*/
    /*if (pid < 0)*/
        /*printf("error");*/
    /*else if (pid == 0) {*/
        /*printf("I am subprocess\n");*/
        /*printf("subpid = %d\n", getpid());*/
    /*} else {*/
        /*printf("I am father of process\n");*/
        /*printf("father pid = %d\n", getpid());*/
    /*}*/

    int status;
    pid_t pid;
    if ((pid = fork()) > 0) {
        while (waitpid(pid, &status, 0) > 0) {
            printf("subprocess dead\n");
            printf("dead stutas %d\n", status);
        }

    } else if (pid == 0) {
        printf("subprocess is running\n");
    } else if (pid < 0) {
        perror("fork error\n");
    }
    /*pid_t origin = getpid();*/
    /*printf("origin = %d", origin);*/
    /*pid_t parent = getppid();*/
    /*printf("parent = %d", parent);*/

    exit(EXIT_SUCCESS);
}
