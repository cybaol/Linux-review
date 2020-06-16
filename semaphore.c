#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
const char* sem_name = "/sem_name";
int main(int argc, char* argv[])
{
    umask(0);
    int sval;
    sem_t* sem = sem_open(sem_name, O_CREAT, 777, 0);
    if (sem == SEM_FAILED) {
        perror("sem_open");
        exit(EXIT_FAILURE);
    }

    sem_getvalue(sem, &sval);
    printf("%d\n", sval);

    /*if(sem_wait(sem) == -1)*/
    /*perror("sem_wait");*/
    /*printf("%d sem_wait succeed!\n", getpid());*/
    /*sem_trywait(sem);*/

    if (sem_post(sem) == -1)
        perror("sem_post");

    sem_getvalue(sem, &sval);
    printf("%d\n", sval);

    sem_close(sem);
    /*sem_unlink(sem_name);*/

    exit(EXIT_SUCCESS);
}
