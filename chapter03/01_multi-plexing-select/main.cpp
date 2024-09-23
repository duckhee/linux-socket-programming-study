#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <memory>
#include <string>
#include <list>
#include <signal.h>


#define BUFFER_MAX                      1024

void CloseAllClient();

void SendBroadCastMsg(char *pszMsg, int size);

void SignalHandler(int sig);

int g_hServerSocket;

std::list<int> g_clientList;
int g_nFdSize = 0;


int main(int argc, char **argv) {
    sockaddr_in serverAddr = {0,};
    /** signal interrupt handler add SIGINT */
    signal(SIGINT, SignalHandler);
    g_hServerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (g_hServerSocket == -1) {
        std::cout << "Failed Create server Socket" << std::endl;
        return -1;
    }

    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(25000);
    serverAddr.sin_family = PF_INET;

    int isBind = bind(g_hServerSocket, (sockaddr *) &serverAddr, sizeof(serverAddr));
    if (isBind == -1) {
        std::cout << "Failed Bind Socket..." << std::endl;
        return -1;
    }

    int isListened = listen(g_hServerSocket, SOMAXCONN);
    if (isListened == -1) {
        std::cout << "Failed Socket Listened..." << std::endl;
        return -1;
    }

    g_clientList.push_back(g_hServerSocket);

    fd_set fdRead;
    std::list<int>::iterator it;

    puts("Start IO MultiPlexing Server...");

    do {
        FD_ZERO(&fdRead);
        for (it = g_clientList.begin(); it != g_clientList.end(); ++it) {
            FD_SET(*it, &fdRead);
            g_nFdSize++;
        }

        /** Select 를 이용한 변화 감지 대기 */
        int result = select(0, &fdRead, NULL, NULL, NULL);
        if (result == -1) {
            puts("Select() Error!");
            break;
        }
        /** 변화 감지 */
        for (int nIndex = 0; nIndex < g_nFdSize + 1; ++nIndex) {
            if (!FD_ISSET(nIndex, &fdRead)) {
                continue;
            }
            /** 서버의 연결 요청일 경우 */
            if (g_hServerSocket == nIndex) {
                sockaddr_in clientAddr = {0,};
                socklen_t clientLen = sizeof(clientAddr);
                int clientSocket = accept(g_hServerSocket, (sockaddr *) &clientAddr, &clientLen);
                /** 제대로 연결 시 등록 진행 */
                if (clientSocket != -1) {
                    FD_SET(clientSocket, &fdRead);
                    g_clientList.push_back(clientSocket);
                    puts("New Client Connection...");
                }
            }
                /** client 데이터 전송일 경우 */
            else {
                char pszBuffer[BUFFER_MAX] = {0,};
                int nReceive = ::recv(nIndex, (char *) pszBuffer, sizeof(pszBuffer), 0);
                /** client 연결 종료일 경우 */
                if (nReceive <= 0) {
                    close(nIndex);
                    FD_CLR(nIndex, &fdRead);
                    g_clientList.remove(nIndex);
                    puts("Client Connection Closed...");
                }
                    /** 메세지가 전달이 된 경우 */
                else {
                    SendBroadCastMsg(pszBuffer, nReceive);
                }
            }
        }
    } while (true);

    CloseAllClient();
    puts("Server Closed...");

    return 0;
}


void CloseAllClient() {
    std::list<int>::iterator it;
    for (it = g_clientList.begin(); it != g_clientList.end(); ++it) {
        ::shutdown(*it, SHUT_RDWR);
        close(*it);
    }
}

void SendBroadCastMsg(char *pszMsg, int size) {
    std::list<int>::iterator it;
    for (it = g_clientList.begin(); it != g_clientList.end(); ++it) {
        ::send(*it, pszMsg, size, 0);
    }
}

void SignalHandler(int sig) {
    std::cout << "SIGNAL NUMBER : " << sig << std::endl;
    CloseAllClient();
    std::cout << "All Client Closed..." << std::endl;
    exit(0);
}