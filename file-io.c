#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/mman.h>
char buffer[1000 + 5];
int main(int argc, char* argv[])
{
    const char* str = "Hello World!";
    int fd = open("a.txt", O_RDWR | O_CREAT);
    lseek(fd, 0, SEEK_SET);
    write(fd, str, strlen(str));
    read(fd, buffer, 1000);
    printf("%s\n", buffer);
    close(fd);
    return 0;
}
