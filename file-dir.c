#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/sysmacros.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
void printDir(char* dir, int depth)
{
    DIR* dp;
    struct dirent* entry;
    struct stat statbuf;
    if ((dp = opendir(dir)) == NULL) {
        perror("opendir");
        exit(EXIT_FAILURE);
    }
    chdir(dir);
    while ((entry = readdir(dp)) != NULL) {
        lstat(entry->d_name, &statbuf);
        if (S_ISDIR(statbuf.st_mode)) {
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;
            printf("%*s%s/\n", depth," " ,entry->d_name);
            printDir(entry->d_name, depth + 4);
        }else {
            printf("%*s%s\n", depth," " ,entry->d_name);
        }
    }
    chdir("..");
    closedir(dp);
}
int main(int argc, char* argv[])
{
    printDir(argv[1], 0);
    printf("Done...\n");
    exit(EXIT_SUCCESS);
}
