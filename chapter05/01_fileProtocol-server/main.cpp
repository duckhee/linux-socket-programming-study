#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <memory>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include "AppProtocol.h"

#define MAX_FILE_PATH                       1024
#define MAX_FILE                            3

void ErrorHandler(const char *msg);

void SendFileList(int hClient);

void SendFile(int hClient, int index);

void GetFilePath(const char *name, char *path);

using namespace std;

SEND_FILE_LIST g_fList = {MAX_FILE};

FILE_INFO g_aFInfo[MAX_FILE] = {
        {0, "Sleep Away.mp3",                4842585},
        {1, "Kalimba.mp3",                   8414449},
        {2, "Maid with the Flaxen Hair.mp3", 4113874}
};

int main(int argc, char **argv) {
    int hServerSock, hClientSock;
    sockaddr_in serverAddr = {0,}, clientAddr = {0,};
    socklen_t clientAddrSize = sizeof(clientAddr);

    hServerSock = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (hServerSock == -1) {
        ErrorHandler("server socket Create Failed...");
    }

    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(25000);
    serverAddr.sin_family = PF_INET;

    int isBind = ::bind(hServerSock, (struct sockaddr *) &serverAddr, sizeof(serverAddr));
    if (isBind == -1) {
        ErrorHandler("Bind Socket Failed...");
    }

    int isListened = ::listen(hServerSock, SOMAXCONN);
    if (isListened == -1) {
        ErrorHandler("Socket Listened Failed...");
    }

    puts("File Socket Server Start!");

    hClientSock = ::accept(hServerSock, (struct sockaddr *) &clientAddr, &clientAddrSize);

    if (hClientSock == -1) {
        ErrorHandler("Failed client socket accept...");
    }

    puts("Client Connected !");

    MY_CMD cmd;

    while (::recv(hClientSock, (char *) &cmd, sizeof(cmd), 0) > 0) {
        switch (cmd.nCode) {
            case CMD_GET_LIST:
                puts("Client Call FILE LIST!");
                SendFileList(hClientSock);
                break;
            case CMD_GET_FILE:
                puts("Client Call File !");
                {
                    FILE_INFO fileInfo;
                    ::recv(hClientSock, (char *) &fileInfo, sizeof(fileInfo), 0);
                    SendFile(hClientSock, fileInfo.nIndex);

                }
                break;
        }
    }
    ::close(hClientSock);
    ::close(hServerSock);
    return 0;
}


void ErrorHandler(const char *msg) {
    printf("ERROR : %s\r\n", msg);
    fflush(stdout);
    exit(1);
}

void SendFileList(int hClient) {
    MY_CMD cmd;
    cmd.nCode = CMD_SND_FILE_LIST;
    cmd.nSize = sizeof(g_fList) + sizeof(g_aFInfo);
    ::send(hClient, (const char *) &cmd, sizeof(cmd), 0);
    ::send(hClient, (const char *) &g_fList, sizeof(g_fList), 0);
    ::send(hClient, (const char *) &g_aFInfo, sizeof(g_aFInfo), 0);
}

void SendFile(int hClient, int index) {
    char pathBuffer[MAX_FILE_PATH] = {0,};

    MY_CMD cmd;
    ErrorData error;

    if (index < 0 || index > 2) {
        cmd.nCode = CMD_ERROR;
        cmd.nSize = sizeof(error);
        error.nErrorCode = 0;
        strcpy(error.szDesc, "Wrong Index Number");

        ::send(hClient, (const char *) &cmd, sizeof(cmd), 0);
        ::send(hClient, (const char *) &error, sizeof(error), 0);
        return;
    }

    cmd.nCode = CMD_BEGIN_FILE;
    cmd.nSize = sizeof(FILE_INFO);
    ::send(hClient, (const char *) &cmd, sizeof(cmd), 0);
    ::send(hClient, (const char *) &g_aFInfo[index], sizeof(FILE_INFO), 0);

    FILE *pFile = NULL;
    GetFilePath(g_aFInfo[index].szFileName, pathBuffer);
    pFile = fopen(pathBuffer, "rb");
    if (pFile == NULL) {
        ErrorHandler("Failed File Open...");
    }

    char pszBuffer[SEND_MAX_BUFFER_SIZE] = {0,};
    int nRead = 0;

    while ((nRead = ::fread(pszBuffer, sizeof(char), SEND_MAX_BUFFER_SIZE, pFile)) > 0) {
        ::send(hClient, pszBuffer, nRead, 0);
    }
    fclose(pFile);
}

void GetFilePath(const char *name, char *path) {
    char executeBuffer[MAX_FILE_PATH] = {0,};
    char tempBuffer[MAX_FILE_PATH] = {0,};
    char *pRemoveStart = nullptr;
    int removeEndIdx = 0;

    realpath(".", executeBuffer);

    pRemoveStart = strstr(executeBuffer, "/cmake");
    if (pRemoveStart == nullptr) {
        cout << "Failed Get Path..." << endl;
        exit(1);
    }

    removeEndIdx = (int) (pRemoveStart - executeBuffer);

    memcpy(tempBuffer, executeBuffer, sizeof(char) * removeEndIdx);
    strcat(tempBuffer, "/chapter05/01_fileProtocol-server/resources/");
    strcat(tempBuffer, name);

    strcpy(path, tempBuffer);
}