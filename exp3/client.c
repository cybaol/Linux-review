#include <errno.h>
#include <fcntl.h>
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#define SERVER_FIFO "/tmp/seqnum_sv"
#define CLIENT_FIFO_TEMPLATE "/tmp/seqnum_cl.%ld"
#define CLIENT_FIFO_NAME_LEN (sizeof(CLIENT_FIFO_TEMPLATE) + 5)
static char clientFifo[CLIENT_FIFO_NAME_LEN];
const char* msgq = "/msgq";
static void removeFifo(void)
{
    unlink(clientFifo);
}
struct request {
    pid_t pid;
    int SeqLen;
};

struct response {
    int SeqNum;
};

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

    int serverFd, clientFd;
    struct request req;
    struct response resp;
    if (argc > 1 && strcmp(argv[1], "--help") == 0) {
        fprintf(stderr, "%s [seq-len...]\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    umask(0);
    snprintf(clientFifo, CLIENT_FIFO_NAME_LEN, CLIENT_FIFO_TEMPLATE, (long)getpid());
    if (mkfifo(clientFifo, S_IRUSR | S_IWUSR | S_IWGRP) == -1 && errno != EEXIST) {
        fprintf(stderr, "mkfifo %s", clientFifo);
        exit(EXIT_FAILURE);
    }
    if (atexit(removeFifo) != 0)
        err_exit("atexit");
    req.pid = getpid();
    req.SeqLen = (argc > 1) ? atoi(argv[1]) : 1;

    serverFd = open(SERVER_FIFO, O_WRONLY);
    if (serverFd == -1) {
        fprintf(stderr, "open %s", SERVER_FIFO);
        exit(EXIT_FAILURE);
    }
    if (write(serverFd, &req, sizeof(struct request)) != sizeof(struct request)) {
        err_exit("can not write to server");
    }
    clientFd = open(clientFifo, O_RDONLY);
    if (clientFd == -1) {
        fprintf(stderr, "open %s", clientFifo);
        exit(EXIT_FAILURE);
    }
    if (read(clientFd, &resp, sizeof(struct response)) != sizeof(struct response)) {
        err_exit("can not response from server");
    }
    printf("%d\n", resp.SeqNum);
    exit(EXIT_FAILURE);
}
