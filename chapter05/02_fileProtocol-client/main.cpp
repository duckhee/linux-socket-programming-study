#include <iostream>
#include <memory>
#include <string>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include "AppProtocol.h"

#define MAX_FILE_PATH                       1024

using namespace std;

void ErrorHandler(const char *msg);


void GetFileList(int clientSock);

void GetFile(int clientSock);

int main(int argc, char **argv) {
    int serverSock;
    sockaddr_in serverAddr = {0,};

    serverSock = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSock == -1) {
        ErrorHandler("Failed Create Socket");
    }

    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverAddr.sin_port = htons(25000);
    serverAddr.sin_family = PF_INET;

    int isConnected = ::connect(serverSock, (struct sockaddr *) &serverAddr, sizeof(serverAddr));
    if (isConnected == -1) {
        ErrorHandler("connect server failed...");
    }

    GetFileList(serverSock);

    GetFile(serverSock);

    ::close(serverSock);

    return 0;
}


void ErrorHandler(const char *msg) {
    printf("ERROR : %s\r\n", msg);
    fflush(stdout);
    exit(1);
}


void GetFileList(int clientSock) {
    /** file list call command */
    MY_CMD cmd = {CMD_GET_LIST, 0};
    ::send(clientSock, (const char *) &cmd, sizeof(cmd), 0);

    /** file list 응답 받기 */
    ::recv(clientSock, (char *) &cmd, sizeof(cmd), 0);
    if (cmd.nCode != CMD_SND_FILE_LIST) {
        ErrorHandler("Failed Get File List...");
    }

    /** file list 에 대한 정보 담기 */
    SEND_FILE_LIST fileList;
    ::recv(clientSock, (char *) &fileList, sizeof(fileList), 0);

    /** 수신한 파일 목록을 출력 -> 파일 데이터 형태로 끊어서 읽는다. */
    /** 출력만 할 용도로 사용 되기 때문에 구조체 하나로 사용을 한다. -> 메모리 절약 */
    FILE_INFO fileInfo;
    for (int i = 0; i < fileList.nIndex; ++i) {
        ::recv(clientSock, (char *) &fileInfo, sizeof(fileInfo), 0);
        printf("[%d]\t%s\t%d\r\n", fileInfo.nIndex, fileInfo.szFileName, fileInfo.dwFileSize);
    }


}

void GetFile(int clientSock) {
    int nIndex = 0;
    printf("수신할 파일의 인덱스(0 ~ 2)를 입력하세요 : ");
    fflush(stdout);
    scanf("%d", &nIndex);

    unsigned char *pCommand = new unsigned char[sizeof(MY_CMD) + sizeof(GET_FILE)];
    MY_CMD *pCmd = (MY_CMD *) pCommand;
    pCmd->nCode = CMD_GET_FILE;
    pCmd->nSize = sizeof(GET_FILE);

    GET_FILE *pFileInfo = (GET_FILE *) (pCommand + sizeof(MY_CMD));
    pFileInfo->nIndex = nIndex;

    ::send(clientSock, (const char *) pCommand, sizeof(MY_CMD) + sizeof(GET_FILE), 0);

    delete[] pCommand;

    MY_CMD cmd = {0,};
    FILE_INFO fileInfo = {0,};

    ::recv(clientSock, (char *) &cmd, sizeof(cmd), 0);
    if (cmd.nCode == CMD_ERROR) {
        ErrorData error = {0,};
        ::recv(clientSock, (char *) &error, sizeof(error), 0);
        ErrorHandler(error.szDesc);
    } else {
        ::recv(clientSock, (char *) &fileInfo, sizeof(fileInfo), 0);
    }

    FILE *pFile = nullptr;
    pFile = fopen(fileInfo.szFileName, "wb");
    if (pFile == nullptr) {
        ErrorHandler("Failed File Open...");
    }

    char byBuffer[SEND_MAX_BUFFER_SIZE] = {0,};
    int nRecv;
    size_t totalRead = 0;
    while ((nRecv = ::recv(clientSock, byBuffer, SEND_MAX_BUFFER_SIZE, 0)) > 0) {
        fwrite(byBuffer, nRecv, 1, pFile);
        totalRead += nRecv;
        putchar('#');

        if (totalRead >= fileInfo.dwFileSize) {
            putchar('\r\n');
            puts("파일 수신 완료!");
            break;
        }
    }

    fclose(pFile);

}