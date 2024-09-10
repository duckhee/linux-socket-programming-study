#include <iostream>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/errno.h>
#include <arpa/inet.h>

using namespace std;


int main(int argc, char **argv) {
    int hClientSocket;
    sockaddr_in serverInfo = {0,};
    socklen_t infoLength;
    char pszBuffer[128] = {0,};
    int nReceiveLength;

    hClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (hClientSocket == -1) {
        cout << "Failed Socket Created..." << endl;
        return -1;
    }

    serverInfo.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverInfo.sin_port = htons(25000);
    serverInfo.sin_family = PF_INET;

    int isConnected = connect(hClientSocket, (sockaddr *) &serverInfo, sizeof(serverInfo));
    if (isConnected == -1) {
        cout << "Failed Connected server ..." << endl;
        close(hClientSocket);
        return -1;
    }

    while (true) {
        fgets(pszBuffer, 128, stdin);
        if (strcmp(pszBuffer, "EXIT\n") == 0) {
            cout << "Connection release" << endl;
            break;
        }
        send(hClientSocket, pszBuffer, 128, 0);
        memset(pszBuffer, '\0', sizeof(pszBuffer));
        recv(hClientSocket, pszBuffer, 128, 0);
        printf("Receive From Server : %s\r\n", pszBuffer);
    }
    shutdown(hClientSocket, SHUT_RDWR);
    close(hClientSocket);
    return 0;
}