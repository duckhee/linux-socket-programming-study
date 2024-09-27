#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <memory.h>
#include <cstring>

#define SOCKET_BUFFER_MAX                  (1024 * 64)
#define PATH_BUFFER_MAX                    1024

void ErrorHandler(const char *msg) {
    printf("ERROR : %s\r\n", msg);
    exit(1);
}

void GetFilePath(const char *name, char *pathBuffer);


int main(int argc, char **argv) {
    int hServer, hClient;
    sockaddr_in serverAddr = {0,}, clientAddr = {0,};
    socklen_t clientAddrLen = sizeof(clientAddr);

    hServer = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (hServer == -1) {
        ErrorHandler("Failed Create Socket");
    }

    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_family = PF_INET;
    serverAddr.sin_port = htons(25000);

    int isBind = bind(hServer, (struct sockaddr *) &serverAddr, sizeof(serverAddr));
    if (isBind == -1) {
        ErrorHandler("Failed Bind socket...");
    }

    int isListen = listen(hServer, SOMAXCONN);
    if (isListen == -1) {
        ErrorHandler("Failed Listen socket...");
    }

    hClient = accept(hServer, (struct sockaddr *) &clientAddr, &clientAddrLen);
    if (hClient == -1) {
        ErrorHandler("Failed Accept client...");
    }


    FILE *fp = NULL;
    /** send buffer */

    char pathBuffer[PATH_BUFFER_MAX] = {0,};
    GetFilePath("SleepAway.zip", pathBuffer);

    fp = fopen(pathBuffer, "rb");

    if (fp == NULL) {
        ErrorHandler("File Open Failed... Check path");
    }


    char byBuffer[SOCKET_BUFFER_MAX] = {0,};
    int nRead, nSent, i = 0;

    while ((nRead = fread(byBuffer, sizeof(char), SOCKET_BUFFER_MAX, fp)) > 0) {
        nSent = send(hClient, byBuffer, nRead, 0);
        printf("[%04d] send data byte : %d\r\n", ++i, nSent);
    }

    usleep(100);


    close(hClient);
    close(hServer);
    puts("클라이언트 연결이 끊겼습니다.");
    /** file 닫기 */
    fclose(fp);
    return 0;
}


void GetFilePath(const char *name, char *pathBuffer) {
    char tempBuffer[PATH_BUFFER_MAX] = {0,};
    char *pRemoveStart = nullptr;
    int removeEndIdx = 0;

    realpath(".", tempBuffer);
    printf("Real Path : %s\r\n", tempBuffer);
    pRemoveStart = strstr(tempBuffer, "/cmake");
    if (pRemoveStart == NULL) {
        ErrorHandler("Failed File Path Get...");
    }
    removeEndIdx = (int) (pRemoveStart - tempBuffer);
    memcpy(pathBuffer, tempBuffer, removeEndIdx);

    strcat(pathBuffer, "/chapter04/01_not-protocol-file-send-server/");
    strcat(pathBuffer, name);
}