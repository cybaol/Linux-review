#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#define BUFSIZE 8
int main(int argc, char* argv[])
{
    int pfd[2]; /* 0 read, 1 write */
    char buf[BUFSIZE];
    int numRead = 0;
    if (pipe(pfd) == -1)
        exit(EXIT_FAILURE);
    switch (fork()) {
    case -1:
        exit(EXIT_FAILURE);
    case 0:
        if (close(pfd[1]) == -1)
            exit(EXIT_FAILURE);
        for (;;) {
            numRead = read(pfd[0], buf, BUFSIZE);
            if (numRead < 0)
                printf("read error\n");
            if (numRead == 0)
                break;
            if (write(STDOUT_FILENO, buf, numRead) != numRead)
                exit(EXIT_FAILURE);
        }
        write(STDOUT_FILENO, "\n", 1);
        if (close(pfd[0]) == -1)
            exit(EXIT_FAILURE);
        exit(EXIT_SUCCESS);
    default:
        if (close(pfd[0]) == -1)
            exit(EXIT_FAILURE);
        if (write(pfd[1], argv[1], strlen(argv[1])) != strlen(argv[1]))
            exit(EXIT_FAILURE);
        if (close(pfd[1]) == -1)
            exit(EXIT_FAILURE);
        wait(NULL);
        exit(EXIT_SUCCESS);
    }
    exit(EXIT_SUCCESS);
}
