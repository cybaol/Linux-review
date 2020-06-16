#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
int numChild = 0;
void destory()
{
    pid_t child;
    while ((child = waitpid(-1, 0, WNOHANG)) > 0) {
        numChild--;
        printf("destory process %d\n", child);
    }
}
int main(int argc, char* argv[])
{
    numChild = atoi(argv[1]);
    const int N = numChild;
    int* children = (int*)malloc(N * sizeof(int));
    struct sigaction act;
    act.sa_handler = destory;
    act.sa_flags = 0;
    sigemptyset(&act.sa_mask);
    sigaction(SIGCHLD, &act, NULL);
    for (int i = 0; i < numChild; ++i) {
        if ((children[i] = fork()) == 0) {
            free(children);
            printf("this is child %d\n", getpid());
            sleep(i + 5);
            exit(EXIT_SUCCESS);
        }
    }
    while (numChild) {
        pause();
    }
    free(children);
    exit(EXIT_SUCCESS);
}
