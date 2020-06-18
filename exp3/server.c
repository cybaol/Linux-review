#include <errno.h>
#include <fcntl.h>
#include <mqueue.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#define SERVER_FIFO "/tmp/seqnum_sv"
#define CLIENT_FIFO_TEMPLATE "/tmp/seqnum_cl.%ld"
#define CLIENT_FIFO_NAME_LEN (sizeof(CLIENT_FIFO_TEMPLATE) + 5)

const char* msgq = "/msgq";
struct request {
    pid_t pid;
    int SeqLen;
};

struct response {
    int SeqNum;
};

void err_exit(const char* msg){

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
    int serverFd, dummyFd, clientFd;
    char clientFifo[CLIENT_FIFO_NAME_LEN];
    struct request req;
    struct response resp;
    int seqNum = 0;

    umask(0);
    if (mkfifo(SERVER_FIFO, S_IRUSR | S_IWUSR | S_IWGRP) == -1 && errno != EEXIST) {
        fprintf(stderr, "mkfifo %s", SERVER_FIFO);
        exit(EXIT_FAILURE);
    }
    serverFd = open(SERVER_FIFO, O_RDONLY);
    if (serverFd == -1) {
        fprintf(stderr, "open %s", SERVER_FIFO);
        exit(EXIT_FAILURE);
    }

    dummyFd = open(SERVER_FIFO, O_WRONLY);
    if (dummyFd == -1) {
        fprintf(stderr, "open %s", SERVER_FIFO);
        exit(EXIT_FAILURE);
    }

    if (signal(SIGPIPE, SIG_IGN) == SIG_ERR) {
        perror("signal");
        exit(EXIT_FAILURE);
    }
    for (;;) {
        if (read(serverFd, &req, sizeof(struct request)) != sizeof(struct request)) {
            fprintf(stderr, "Error reading request: discarding\n");
            continue;
        }
        snprintf(clientFifo, CLIENT_FIFO_NAME_LEN, CLIENT_FIFO_TEMPLATE, (long)req.pid);
        clientFd = open(clientFifo, O_WRONLY);
        if (clientFd == -1)
            fprintf(stderr, "open %s", clientFifo);
        resp.SeqNum = seqNum;
        if (write(clientFd, &resp, sizeof(struct response)) != sizeof(struct response)) {
            fprintf(stderr, "Error writing to FIFO %s\n", clientFifo);
        }
        if (close(clientFd) == -1) {
            fprintf(stderr, "close");
        }
        seqNum += req.SeqLen;
    }
    exit(EXIT_SUCCESS);
}
