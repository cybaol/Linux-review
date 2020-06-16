#include <error.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
void sigHandler()
{
    printf("ouch!\n");
    /*signal(SIGINT, SIG_DFL);*/
}
int main(int argc, char* argv[])
{
    if ((signal(SIGINT, sigHandler)) == SIG_ERR) {
        perror("signal error\n");
    }
    for (int i = 0; i < 100; ++i) {
        printf("%d\n", i);
        sleep(3);
    }
    exit(EXIT_SUCCESS);
}
