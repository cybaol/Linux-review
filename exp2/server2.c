#include "cliserv2.h"

int main(int argc, char** argv)
{
    int fd, index, lastnoverflow, temp;
    long offset;
    struct shmstruct* ptr;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <IPC object name.\n>", argv[0]);
        exit(EXIT_FAILURE);
    }

    /* 4create shm, set its size, map it, close descriptor */
    shm_unlink(argv[1]); /* OK if this fails */
    fd = shm_open(argv[1], O_RDWR | O_CREAT | O_EXCL, FILE_MODE);
    ptr = mmap(NULL, sizeof(struct shmstruct), PROT_READ | PROT_WRITE,
        MAP_SHARED, fd, 0);
    ftruncate(fd, sizeof(struct shmstruct));
    close(fd);

    /* 4initialize the array of offsets */
    for (index = 0; index < NMESG; index++)
        ptr->msgoff[index] = index * MESGSIZE;

    /* sem_init 用来初始化共享存储区中的信号量 */
    sem_init(&ptr->mutex, 1, 1);
    sem_init(&ptr->nempty, 1, NMESG);
    sem_init(&ptr->nstored, 1, 0);
    sem_init(&ptr->noverflowmutex, 1, 1);

    /* 4this program is the consumer */
    index = 0;
    lastnoverflow = 0;
    for (;;) {
        sem_wait(&ptr->nstored); //is there something consumable?
        sem_wait(&ptr->mutex);   // entering the critical section
        offset = ptr->msgoff[index];
        printf("index = %d: %s\n", index, &ptr->msgdata[offset]);
        if (++index >= NMESG)
            index = 0;          /* circular buffer */
        sem_post(&ptr->mutex);  // leaving the critical section
        sem_post(&ptr->nempty); // one slot available to producers

        sem_wait(&ptr->noverflowmutex); // get times of overflow
        temp = ptr->noverflow;          /* don't printf while mutex held */
        sem_post(&ptr->noverflowmutex);
        if (temp != lastnoverflow) {
            printf("noverflow = %d\n", temp);
            lastnoverflow = temp; // update
        }
    }

    exit(EXIT_SUCCESS);
}
