#include <iostream>
#include <string.h>
#include <memory>
#include <string>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <fcntl.h>

void ErrorHandler(const char *msg);

int main(int argc, char **argv) {
    int broadCastSocket;
    sockaddr_in broadAddr;
    broadCastSocket = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (broadCastSocket == -1) {
        ErrorHandler("Failed Create UDP Socket");
    }

    broadAddr.sin_family = PF_INET;
    broadAddr.sin_port = htons(25000);
    broadAddr.sin_addr.s_addr = htonl(INADDR_BROADCAST);

    /** socket 에 대한 설정 변경 */
    int nOption = 1;
    ::setsockopt(
            broadCastSocket,
            SOL_SOCKET,
            SO_BROADCAST,
            (const char *) &nOption,
            sizeof(nOption)
    );

    char szBuffer[128] = {0,};
    while (true) {
        putchar('>');
        gets(szBuffer);
        if (strcmp(szBuffer, "EXIT") == 0) {
            printf("UDP Socket Exit\r\n");
            break;
        }
        // 방송 주소로 메세지 전송
        // 모든 Peer들이 동시에 메시지를 수신한다.
        ::sendto(broadCastSocket, szBuffer, sizeof(szBuffer), 0, (struct sockaddr *) &broadAddr, sizeof(broadAddr));
    }
    close(broadCastSocket);
    return 0;
}

void ErrorHandler(const char *msg) {
    fputs(msg, stderr);
    fputc('\r\n', stderr);
    fflush(stderr);
    exit(1);
}