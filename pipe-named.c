#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#define MAXSIZE 128
const char* fifo = "fifo";
int main(int argc, char* argv[])
{
    umask(0);
    const char* buf = "Hello World!";
    /*mkfifo(fifo, S_IRUSR | S_IWUSR | S_IWGRP);*/
    int fd = open(fifo, O_RDWR);
    /*write(fd, buf, strlen(buf));*/
    char str[MAXSIZE];
    read(fd, str, MAXSIZE);
    printf("%s\n", str);
    close(fd);
    /*unlink(fifo);*/
    exit(EXIT_SUCCESS);
}
