#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUF_SIZE 1024

void Error_Func(char* ErrorMessage);
int main(int argc, char* argv[])
{
    int sock;
    struct sockaddr_in serv_addr;
    char message[BUF_SIZE];
    int str_len;
    if (argc != 3) {
        printf("Usage : %s <IP> <port>", argv[0]);
    }

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == -1)
        Error_Func("socket() error");
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
        Error_Func("connect() error");
    else
        puts("Connected success.....");

    while (1) {
        fputs("Inputs message(Q to exit):", stdout);
        fgets(message, BUF_SIZE, stdin);
        if (!strcmp(message, "Q\n") || !strcmp(message, "q\n"))
            break;
        write(sock, message, strlen(message));
        str_len = read(sock, message, BUF_SIZE - 1);
        message[str_len] = 0;
        printf("Message From Server : %s\n", message);
    }
    close(sock);
    return 0;
}
void Error_Func(char* ErrorMessage)
{
    fputs(ErrorMessage, stderr);
    fputc('\n', stderr);
    exit(EXIT_FAILURE);
}
