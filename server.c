#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>

#define BUF_SIZE            10000
#define FILE_NAME_SIZE      BUF_SIZE
#define FILE_LENGTH_SIZE    BUF_SIZE

void error_handling(char *message);

int  main(int argc, char *argv[]) {
    int  serverSock, clientSock;
    char file_name[FILE_NAME_SIZE];
    char file_length[FILE_LENGTH_SIZE];
    char message[BUF_SIZE];

    struct sockaddr_in serverAdr, clientAdr;
    socklen_t clientAdr_sz;

    FILE *fp;
    long  nsize = 0;
    long  ssize = 0;

    long fsize = 0;
    long rsize = 0;

    long total_size = 0;

    if (argc != 2) {
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }

    serverSock = socket(PF_INET, SOCK_STREAM, 0);

    if (serverSock == -1) error_handling("socket() Error");
    memset(&serverAdr, 0, sizeof(serverAdr));
    serverAdr.sin_family      = AF_INET;
    serverAdr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAdr.sin_port        = htons(atoi(argv[1])); // port

    if (bind(serverSock, (struct sockaddr *)&serverAdr, sizeof(serverAdr)) == -1) error_handling("bind() error");

    if (listen(serverSock, 5) == -1) error_handling("listen() Error");

    while (1) {
        clientAdr_sz = sizeof(clientAdr);
        clientSock   = accept(serverSock, (struct sockaddr *)&clientAdr, &clientAdr_sz);

        if (clientSock == -1) error_handling("accept() Error");

        // title
        nsize      = FILE_NAME_SIZE;
        rsize      = 0;
        total_size = 0;

        do {
            total_size += rsize;
            nsize      -= rsize;
            rsize       = recv(clientSock, file_name + total_size, nsize, 0);
        } while (nsize != rsize);
        fp = fopen(file_name, "wb");

        printf("Send Image name : %s\n", file_name);

        while (rsize = recv(clientSock, message, BUF_SIZE, 0)) {
            fwrite(message, sizeof(char), rsize, fp);
        }

        printf("SUCESS\n");

        fclose(fp);

        close(clientSock);
    }

    close(serverSock);
    return 0;
}

void error_handling(char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

void clear_buffer(char *buffer) {
    int i = 0;

    for (i = 0; i < BUF_SIZE; i++) buffer[i] = 0;
}
