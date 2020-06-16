#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
const char* msgq = "/msgq";
void err_exit(const char* msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}

int main(int argc, char* argv[])
{
    mqd_t mqd;
    unsigned int prio, priority = 1;
    struct mq_attr attr, *attrp;

    attrp = NULL;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = 2048;
    int flags = O_RDWR | O_CREAT;
    mqd = mq_open(msgq, flags, S_IRUSR | S_IWUSR, attrp);
    if (mqd == (mqd_t)-1) {
        err_exit("mq_open");
    }
    if (mq_getattr(mqd, &attr) == -1) {
        err_exit("mq_getattr");
    }
    void* buffer = malloc(attr.mq_msgsize);
    const char* buf = "Hello World!";
    int numWrite = mq_send(mqd, buf, strlen(buf), priority);
    if(numWrite == -1){
        err_exit("mq_send");
    }
    int numRead = mq_receive(mqd, buffer, attr.mq_msgsize, &prio);
    if (numRead == -1) {
        err_exit("mq_receive");
    }
    printf("Read %ld bytes priority = %u\n", (long)numRead, prio);
    if (write(STDOUT_FILENO, buffer, numRead) == -1) {
        err_exit("write");
    }
    write(STDOUT_FILENO, "\n", 1);
    free(buffer);
    mq_close(mqd);
    /*if (mq_unlink(msgq) == -1) {*/
        /*err_exit("mq_close");*/
    /*}*/
    exit(EXIT_SUCCESS);
}
