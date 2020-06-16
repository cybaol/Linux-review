#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
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
    return 0;
}
