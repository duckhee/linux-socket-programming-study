#include <iostream>
#include <memory.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

#define BUFFER_MAX          128


int main(int argc, char **argv) {
    int connectSocket, dataSocket;
    sockaddr_in hostAddr, clientAddr;
    socklen_t clientInfoSize = sizeof(clientAddr);
    /** 데이터를 저장할 buffer */
    char dataBuffer[BUFFER_MAX] = {0,};
    size_t receiveLength = 0, sendLength = 0;
    /** 연결을 위한 소켓 생성 */
    connectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    /** socket 생성 실패 */
    if (connectSocket == -1) {
        puts("ERROR: Failed Create Socket");
        return 0;
    }
    /** host addr 초기화 */
    memset(&hostAddr, '\0', sizeof(hostAddr));
    hostAddr.sin_family = PF_INET;
    hostAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    hostAddr.sin_port = htons(25000);
    /** socket 을 kernel 에 연결 */
    int isBind = bind(connectSocket, (sockaddr *) &hostAddr, sizeof(hostAddr));
    /** bind failed */
    if (isBind == -1) {
        puts("ERROR: Failed Socket Binding");
        close(connectSocket);
        return 0;
    }
    /** 접속 대기 -> 대기 큐에 대한 크기를 OS에서 알아서 할당을 핻주기 위한 설정 값 SDMAXCONN */
    int isListen = listen(connectSocket, SOMAXCONN);
    /** 접속 대기 실패 시 */
    if (isListen == -1) {
        puts("ERROR: Failed Socket Listening");
        return 0;
    }

    /** client socket address */
    memset(&clientAddr, '\0', sizeof(clientAddr));
    /** client socket connect success */
    while ((dataSocket = accept(connectSocket, (sockaddr *) &clientAddr, &clientInfoSize)) != -1) {
        puts("new Client connected");
        /** receive data and echo send */
        while ((receiveLength = recv(dataSocket, dataBuffer, BUFFER_MAX, 0))) {
            sendLength = send(dataSocket, dataBuffer, receiveLength, 0);
            puts(dataBuffer);
            memset(dataBuffer, '\0', sizeof(dataBuffer));
        }
        /** 데이터 전송 수신에 대한 모든 통신 종료 */
        shutdown(dataSocket, SHUT_RDWR);
        close(dataSocket);
        puts("client connection closed");
    }

    close(connectSocket);

    return 0;
}