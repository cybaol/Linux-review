#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#define NLOOP 5000
int s = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
void* sum()
{
    for (int i = 0; i < NLOOP; ++i) {
        pthread_mutex_lock(&mutex);
        ++s;
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}
int main(int argc, char* argv[])
{
    pthread_t tidA, tidB;
    pthread_create(&tidA, 0, &sum, NULL);
    pthread_create(&tidB, 0, &sum, NULL);

    pthread_join(tidA, NULL);
    pthread_join(tidB, NULL);

    printf("s = %d\n", s);
    exit(EXIT_SUCCESS);
}
