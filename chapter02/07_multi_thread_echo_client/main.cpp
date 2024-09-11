#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <arpa/inet.h>

using namespace std;

int main(int argc, char **argv) {
    int clientSocket;
    sockaddr_in serverAddr = {0,};
    char pszBuffer[128] = {0,};
    socklen_t nReceive;
    clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (clientSocket == -1) {
        cout << "Failed Created Socket..." << endl;
        return -1;
    }

    serverAddr.sin_family = PF_INET;
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverAddr.sin_port = htons(25000);

    int isConnected = connect(clientSocket, (sockaddr *) &serverAddr, sizeof(serverAddr));

    if (isConnected == -1) {
        cout << "Failed Connected server..." << endl;
        close(clientSocket);
        return -1;
    }

    while (true) {
        puts("server connected!");
        fgets(pszBuffer, 128, stdin);
        if (strcmp(pszBuffer, "EXIT\n") == 0) {
            cout << "Connection close..." << endl;
            break;
        }
        send(clientSocket, pszBuffer, 128, 0);
        memset(pszBuffer, '\0', sizeof(pszBuffer));
        recv(clientSocket, pszBuffer, 128, 0);
        printf("Receive From Server : %s\r\n", pszBuffer);
    }
    shutdown(clientSocket, SHUT_RDWR);
    close(clientSocket);
    return 0;
}