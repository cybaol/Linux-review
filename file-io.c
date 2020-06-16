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
    /*const int bufsize = atoi(argv[2]);*/
    const char* str = "My ship is solid!";
    int fd = open(argv[1], O_RDWR | O_CREAT | O_APPEND);
    /*lseek(fd, 0, SEEK_END);*/
    char* p = mmap(0, 100, PROT_READ, MAP_SHARED, fd, 0);
    write(fd, str, strlen(str));
    int rdsize = read(fd, buffer, 1000);
    if(rdsize == -1)
        printf("read error");
    printf("%s\n", buffer);
    close(fd);
    return 0;
}
