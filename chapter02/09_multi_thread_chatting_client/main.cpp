#include <iostream>
#include <unistd.h>
#include <string>
#include <memory.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>

void *ReceiveHandler(void *pParam);

using namespace std;

int main(int argc, char **argv) {
    int clientSocket;
    pthread_t thread;
    sockaddr_in serverAddr = {0,};
    char pszBuffer[128] = {0,};

    clientSocket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    serverAddr.sin_family = PF_INET;
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverAddr.sin_port = htons(25000);

    int isConnected = ::connect(clientSocket, (sockaddr *) &serverAddr, sizeof(serverAddr));
    if (isConnected == -1) {
        cout << "Failed Connect server ..." << endl;
        return -1;
    }
    puts("Connected Server!");
    pthread_create(
            &thread,
            NULL,
            &ReceiveHandler,
            &clientSocket
    );
    pthread_detach(thread);
    while (true) {
        fgets(pszBuffer, 128, stdin);
        if (strcmp(pszBuffer, "EXIT\n") == 0) {
            cout << "Close Connection..." << endl;
            break;
        }
        ::send(clientSocket, pszBuffer, 128, 0);
        memset(pszBuffer, '\0', sizeof(pszBuffer));
    }
    ::shutdown(clientSocket, SHUT_RDWR);
    ::close(clientSocket);
    return 0;
}


void *ReceiveHandler(void *pParam) {
    int clientSocket = *(int *) pParam;
    int nReceive = 0;
    char receiveBuffer[128] = {0,};
    while ((nReceive = ::recv(clientSocket, receiveBuffer, 128, 0)) > 0) {
        printf("-> %s\r\n", receiveBuffer);
        memset(receiveBuffer, '\0', sizeof(receiveBuffer));
    }
    fputs("Close Receive Socket...", stdout);
    ::shutdown(clientSocket, SHUT_RD);
//    ::close(clientSocket);
    return 0;
}