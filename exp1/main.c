#include <stdio.h>
int main()
{
    FILE* fp;
    char buffer[80];
    fp = popen("cat ~/a.txt", "r");
    fgets(buffer, sizeof(buffer), fp);
    printf("%s", buffer);
    pclose(fp);
    return 0;
}
