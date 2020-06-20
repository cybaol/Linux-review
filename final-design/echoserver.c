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
    int serv_sock;
    int clnt_sock;

    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size;

    char message[BUF_SIZE];
    int str_len, i;

    if (argc != 2) {
        printf("Usage : %s <port> \n", argv[0]);
        exit(1);
    }

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (serv_sock == -1)
        Error_Func("socket() error");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if (bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
        Error_Func("bind() error");

    if (listen(serv_sock, 5) == -1)
        Error_Func("listen() error");

    clnt_addr_size = sizeof(clnt_addr);

    for (i = 0; i < 5; i++) {
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
        while ((str_len = read(clnt_sock, message, BUF_SIZE)) != 0) {
            printf("received the message from client: \n %s", message);
            write(clnt_sock, message, str_len);
        }

        close(clnt_sock);
    }
    close(serv_sock);

    return 0;
}

void Error_Func(char* ErrorMessage)
{
    fputs(ErrorMessage, stderr);
    fputc('\n', stderr);
    exit(1);
}
