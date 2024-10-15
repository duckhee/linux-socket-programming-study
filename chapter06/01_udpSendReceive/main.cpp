#include <iostream>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <memory>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <pthread.h>

void ErrorHandle(const char *msg);

void *pSendToThread(void *pParam);

char g_szRemoteAddress[32];
int g_nRemotePort;
int g_nLocalPort;


int main(int argc, char **argv) {
    pthread_t sendThread = {0,};
    int serverSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (serverSocket == -1) {
        ErrorHandle("Failed UDP Socket Created...");
    }
    printf("원격지 IP주소를 입력하세요.: ");
    gets(g_szRemoteAddress);
    fflush(stdin);
    printf("원격지 포트번호를 입력하세요.: ");
    scanf("%d", &g_nRemotePort);
    fflush(stdin);
    printf("로컬 포트번호를 입력하세요.: ");
    scanf("%d", &g_nLocalPort);

    /** UDP Socket Binding */
    sockaddr_in addr = {0,};
    addr.sin_family = PF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(g_nLocalPort);
    socklen_t addrSize = sizeof(addr);
    printf("[fd-serverSocket:%d][ip:port]%s:%d\r\n", serverSocket, inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
    int isBind = ::bind(serverSocket, (struct sockaddr *) &addr, addrSize);
    if (isBind == -1) {
        ErrorHandle("UDP Socket Bind Error...");
    }

    pthread_create(&sendThread, NULL, &pSendToThread, (void *) serverSocket);
    //
    pthread_detach(sendThread);
    /** 수신 버퍼 */
    char szBuffer[128];
    sockaddr_in remoteAddr;
    socklen_t nSocket = sizeof(remoteAddr);
    long nReceive = 0;
    while ((nReceive = recvfrom(serverSocket, szBuffer, sizeof(szBuffer), 0, (struct sockaddr *) &remoteAddr,
                                &nSocket)) > 0) {
//        printf("[%ldbyte]-> %s\r\n", nReceive, szBuffer);
        printf("-> %s\r\n", szBuffer);
        memset(szBuffer, '\0', sizeof(szBuffer));

    }

    puts("UDP Protocol End\r\n");
    close(serverSocket);

    return 0;
}

void ErrorHandle(const char *msg) {
    fputs(msg, stderr);
    fputc('\r\n', stderr);
    fflush(stderr);
    exit(1);
}

void *pSendToThread(void *pParam) {
//    int uSocket = *(int *) &pParam;
//    printf("UDP SEND THREAD client socket : %d\r\n", uSocket);
    int clientSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (clientSocket == -1) {
        ErrorHandle("UDP Socket Create Failed...");
    }
    /** send buffer size */
    char szBuffer[128];
    /** UDP 전송을 위한 주소 */
    sockaddr_in remoteAddr = {0,};
    remoteAddr.sin_addr.s_addr = inet_addr(g_szRemoteAddress);
    remoteAddr.sin_port = htons(g_nRemotePort);
    remoteAddr.sin_family = PF_INET;

    while (true) {
        memset(szBuffer, '\0', sizeof(szBuffer));
        gets(szBuffer);
        if (strcmp(szBuffer, "EXIT") == 0) {
//            printf("UDP Socket Exit\r\n");
            break;
        }
        /** UDP 전송 */
        ::sendto(clientSocket, szBuffer, strlen(szBuffer) + 1, 0, (struct sockaddr *) &remoteAddr, sizeof(remoteAddr));

    }
//    printf("close socket\r\n");


    close(*(int *) &pParam);
//    printf("socket fd : %d\r\n", *(int *) &pParam);
    close(clientSocket);
    return (void *) nullptr;
}
