#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <memory.h>
#include <string.h>

using namespace std;

void *EchoServiceThread(void *);

int main(int argc, char **argv) {
    int serverSocket, clientSocket;
    sockaddr_in serverAddr = {0,}, clientAddr = {0,};
    socklen_t clientAddrLen;
    pthread_t thread;

    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == -1) {
        cout << "Failed socket created..." << endl;
        return -1;
    }

    serverAddr.sin_family = PF_INET;
    serverAddr.sin_port = htons(25000);
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    int isBind = bind(serverSocket, (sockaddr *) &serverAddr, sizeof(serverAddr));
    if (isBind == -1) {
        cout << "Failed Bind Socket..." << endl;
        close(serverSocket);
        return -1;
    }

    int isListened = listen(serverSocket, SOMAXCONN);

    if (isListened == -1) {
        cout << "Failed socket Listen..." << endl;
        close(serverSocket);
        return -1;
    }
    cout << "Server start !" << endl;
    while ((clientSocket = accept(serverSocket, (sockaddr *) &clientAddr, &clientAddrLen)) != -1) {
        cout << "client socket : " << clientSocket << endl;
        pthread_create(
                &thread,
                NULL,
                EchoServiceThread,
                &clientSocket
        );
        pthread_detach(thread);
    }
    shutdown(serverSocket, SHUT_RDWR);
    close(serverSocket);
    return 0;
}

void *EchoServiceThread(void *pSocket) {
    cout << "Thread Start" << endl;
    int clientSocket = *((int *) pSocket);

    cout << "[Thread] Client socket : " << clientSocket << endl;
    char pszBuffer[128] = {0,};
    socklen_t nReceive = 0;

    fputs("new client Connected!\r\n", stdout);
    while ((nReceive = recv(clientSocket, pszBuffer, 128, 0)) > 0) {
        send(clientSocket, pszBuffer, sizeof(pszBuffer), 0);
        fputs(pszBuffer, stdout);
        memset(pszBuffer, '\0', sizeof(pszBuffer));
    }
    cout << "Close Client socket..." << endl;
    close(clientSocket);
    return 0;
}