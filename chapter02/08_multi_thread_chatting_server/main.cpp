#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/errno.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <signal.h>
#include <memory.h>
#include <list>


pthread_mutex_t g_cs;
int g_serverSocket;
std::list<int> g_clientList;

bool AddClientSocket(int clientSocket);

void SendMsg(const char *pszBuffer);

void *ClientHandler(void *pParam);

void SignalHandler(int sig);

void ShutDown();

int main(int argc, char **argv) {
    /** mutex 초기화 */
    pthread_mutex_init(&g_cs, NULL);
    pthread_t thread;
    int clientSocket;
    sockaddr_in serverAddr = {0,}, clientAddr = {0,};
    socklen_t clientAddrLen = sizeof(clientAddr);

    /** signal interrupt handler add SIGINT */
    signal(SIGINT, SignalHandler);

    g_serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (g_serverSocket == -1) {
        std::cout << "Failed Server Socket Created ..." << std::endl;
        return -1;
    }

    serverAddr.sin_family = PF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
//    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverAddr.sin_port = htons(25000);

    int isBind = bind(g_serverSocket, (sockaddr *) &serverAddr, sizeof(serverAddr));
    if (isBind == -1) {
        std::cout << "Failed bind server..." << std::endl;
        close(g_serverSocket);
        return -1;
    }

    int isListened = listen(g_serverSocket, SOMAXCONN);
    if (isListened == -1) {
        std::cout << "Failed Listened server socket ..." << std::endl;
        close(g_serverSocket);
        return -1;
    }


    while ((clientSocket = accept(g_serverSocket, (sockaddr *) &clientAddr, &clientAddrLen)) != -1) {
        if (AddClientSocket(clientSocket) != true) {
            puts("Failed Client Socket Add...");
            ShutDown();
            break;
        }
        pthread_create(
                &thread,
                NULL,
                &ClientHandler,
                &clientSocket
        );
        pthread_detach(thread);
    }
    ShutDown();
    return 0;
}


bool AddClientSocket(int clientSocket) {
    pthread_mutex_lock(&g_cs);
    g_clientList.push_back(clientSocket);
    pthread_mutex_unlock(&g_cs);
    return true;
}

void SendMsg(const char *pszBuffer) {
    size_t nLength = sizeof(pszBuffer);
    std::list<int>::iterator it;
    pthread_mutex_lock(&g_cs);
    for (it = g_clientList.begin(); it != g_clientList.end(); ++it) {
        send(*it, pszBuffer, sizeof(char) * nLength + 1, 0);
    }
    pthread_mutex_unlock(&g_cs);
}

void *ClientHandler(void *pParam) {
    int clientSocket = *(int *) pParam;
    char pszBuffer[128] = {0,};
    int nReceive = 0;
    puts("New Client Connected!");
    while ((nReceive = recv(clientSocket, pszBuffer, 128, 0)) > 0) {
        printf("Receive From Client : %s\r\n", pszBuffer);
        SendMsg(pszBuffer);
        memset(pszBuffer, '\0', sizeof(pszBuffer));
    }
    puts("Client Closed...");
    pthread_mutex_lock(&g_cs);
    g_clientList.remove(clientSocket);
    pthread_mutex_unlock(&g_cs);
    close(clientSocket);
    return 0;
}

void ShutDown() {
    std::list<int>::iterator it;
    ::shutdown(g_serverSocket, SHUT_RDWR);
    pthread_mutex_lock(&g_cs);
    for (it = g_clientList.begin(); it != g_clientList.end(); ++it) {
        close(*it);
    }
    g_clientList.clear();
    pthread_mutex_unlock(&g_cs);
    puts("Client All Closed...");
    usleep(100);
    pthread_mutex_destroy(&g_cs);
    close(g_serverSocket);
}

void SignalHandler(int sig) {
    std::cout << "SIGNAL NUMBER : " << sig << std::endl;
    ShutDown();
    std::cout << "Close Server Program..." << std::endl;
    exit(0);

}