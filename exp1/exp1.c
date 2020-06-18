#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#define SHELL "/bin/sh"
#define BUFSIZE 8
const char* str = "Hello World!";
static pid_t* children = NULL;
int pfd[2];
char buf[BUFSIZE];
//create child process
FILE* mypopen(const char* command, const char* type)
{
    if ((*children = fork()) == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    if (pipe(pfd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    if (type[0] == 'r') {
        if (*children == 0) {
            if (close(pfd[0]) == -1) {
                perror("r pfd[0] close");
                exit(EXIT_FAILURE);
            }
            dup2(pfd[1], STDOUT_FILENO | STDERR_FILENO);
            /*execl(SHELL, command);*/
            write(pfd[1], str, strlen(str));
            if (close(pfd[1]) == -1) {
                perror("r pfd[1] close");
                exit(EXIT_FAILURE);
            }
        } else {
            if (close(pfd[1]) == -1) {
                perror("r pfd[1] close");
                exit(EXIT_FAILURE);
            }
            for (;;) {
                int numRead = read(pfd[0], buf, BUFSIZE);
                if (numRead == 0) {
                    break;
                }
                if (write(STDOUT_FILENO, buf, strlen(buf)) != strlen(buf)) {
                    perror("write");
                    exit(EXIT_FAILURE);
                }
            }
            write(STDOUT_FILENO, "\n", 1);
            if (close(pfd[0]) == -1) {
                perror("r pfd[0] close");
                exit(EXIT_FAILURE);
            }
        }
    } else if (type[0] == 'w') {
        if (*children == 0) {

        } else {
        }
    }
    return NULL;
}

//destory child process
/*int mypclose(FILE* stream)*/
/*{*/
    /*pclose(stream);*/
    /*while ((waitpid(*children, NULL, 0)) > 0) {*/
        /*printf("child destory successfully!\n");*/
    /*}*/
    /*return 0;*/
/*}*/

int main(int argc, char* argv[])
{
    FILE* fp = mypopen(argv[1], argv[2]);
    pclose(fp);
    while ((waitpid(*children, NULL, 0)) > 0) {
        printf("child destory successfully!\n");
    }
    exit(EXIT_SUCCESS);
}
