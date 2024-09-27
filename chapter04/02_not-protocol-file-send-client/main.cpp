#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <arpa/inet.h>

#define SOCKET_BUFFER_MAX               (1024 * 64)

void ErrorHandler(const char *msg) {
    printf("ERROR : %s\r\n", msg);
    exit(1);
}


int main(int argc, char **argv) {
    int hSocket;
    sockaddr_in serverAddr = {0,};

    hSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (hSocket == -1) {
        ErrorHandler("Failed Create Socket...");
    }

    serverAddr.sin_family = PF_INET;
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverAddr.sin_port = htons(25000);

    int isConnected = connect(hSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));

    if (isConnected == -1) {
        ErrorHandler("Failed Connected Server...");
    }

    puts("*** 파일 수신을 시작합니다.***\r\n");
    FILE *fp = NULL;
    fp = fopen("receiveData.zip", "wb");

    if (fp == NULL) {
        ErrorHandler("Failed File Open...");
    }

    char byBuffer[SOCKET_BUFFER_MAX] = {0,};
    int nReceive;

    while ((nReceive = recv(hSocket, byBuffer, SOCKET_BUFFER_MAX, 0)) > 0) {
        fwrite(byBuffer, nReceive, 1, fp);
        putchar('#');
        fflush(stdout);
    }

    fclose(fp);

    printf("\r\n*** 파일 송수신 종료***\r\n");

    close(hSocket);

    return 0;
}