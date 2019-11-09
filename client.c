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
void clear_buffer(char *buffer);

int  main(int argc, char *argv[]) {
    int  sock;
    char fileName[FILE_NAME_SIZE];
    char fileLength[FILE_LENGTH_SIZE];
    char message[BUF_SIZE];
    struct sockaddr_in serverAdr;

    FILE *fp;
    long  fsize = 0;
    long  rsize = 0;

    if (argc != 4) {
        printf("Usage : %s <IP> <port> <file_name>\n", argv[0]);
        exit(1);
    }

    sock = socket(PF_INET, SOCK_STREAM, 0);

    if (sock == -1) error_handling("socket() error~");

    memset(&serverAdr, 0, sizeof(serverAdr));
    serverAdr.sin_family      = AF_INET;
    serverAdr.sin_addr.s_addr = inet_addr(argv[1]);  // ip address
    serverAdr.sin_port        = htons(atoi(argv[2])); // port

    if (connect(sock, (struct sockaddr *)&serverAdr, sizeof(serverAdr)) == -1) error_handling("connect() error");
    else puts("Connected......");

    fp = fopen(argv[3], "rb");
    fseek(fp, 0, SEEK_END);
    fsize = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    // title
    strcpy(fileName, argv[3]);
    send(sock, fileName, FILE_NAME_SIZE, 0);

    printf("Send Image | name : %s | size : %ld\n", fileName, fsize);

    while (fsize > 0) {
        rsize = fread(message, 1, BUF_SIZE, fp);
        send(sock, message, rsize, 0);
        fsize -= rsize;
        clear_buffer(message);
    }

    fclose(fp);
    close(sock);
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
