#include <iostream>
#include <memory>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <fcntl.h>

void ErrorHandler(const char *msg);

int main(int argc, char **argv) {
    int bSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (bSocket == -1) {
        ErrorHandler("Failed UDP Socket Created");
    }

    sockaddr_in bAddr = {0,};
    bAddr.sin_family = PF_INET;
    bAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    bAddr.sin_port = htons(25000);

    if (::bind(bSocket, (struct sockaddr *) &bAddr, sizeof(bAddr)) == -1) {
        ErrorHandler("ERROR: 소켓에 IP주소와 포트를 바인딩할 수 없습니다.");
    }

    char szBuffer[128] = {0,};
    while (::recvfrom(bSocket, szBuffer, sizeof(szBuffer), 0, NULL, 0) >= 0) {
        printf(":%s\n", szBuffer);
        memset(szBuffer, 0, sizeof(szBuffer));
    }

    close(bSocket);
    return 0;
}

void ErrorHandler(const char *msg) {
    fputs(msg, stderr);
    fputc('\r\n', stderr);
    fflush(stderr);
    exit(1);
}