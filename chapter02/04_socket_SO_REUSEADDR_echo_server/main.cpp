#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/errno.h>
#include <memory.h>

using namespace std;


int main(int argc, char **argv) {
    int hServerSocket, hClientSocket;
    sockaddr_in serverInfo = {0,}, clientInfo = {0,};
    socklen_t serverInfoLength, clientInfoLength;
    int nReceiveLength;
    char pszBuffer[128] = {0,};

    hServerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (hServerSocket == -1) {
        cout << "Failed Socket Created..." << endl;
        return -1;
    }
    int nOpt = 1;
    setsockopt(hServerSocket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, (void *) &nOpt, sizeof(nOpt));

    serverInfo.sin_family = PF_INET;
//    serverInfo.sin_addr.s_addr = htonl(INADDR_ANY);
    serverInfo.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverInfo.sin_port = htons(25000);
    serverInfoLength = sizeof(serverInfo);
    int isBind = ::bind(hServerSocket, (sockaddr *) &serverInfo, serverInfoLength);

    if (isBind == -1) {
        cout << "Failed Bind Server Socket..." << endl;
        close(hServerSocket);
        return -1;
    }

    int isListened = listen(hServerSocket, SOMAXCONN);

    if (isListened == -1) {
        cout << "Failed server socket listened..." << endl;
        return -1;
    }

    while ((hClientSocket = accept(hServerSocket, (sockaddr *) &clientInfo, &clientInfoLength)) != -1) {
        cout << "Connected new Client !" << endl;

        while ((nReceiveLength = recv(hClientSocket, pszBuffer, 128, 0)) > 0) {
            send(hClientSocket, pszBuffer, 128, 0);
            printf("Receive From Client : %s\r\n", pszBuffer);
            memset(pszBuffer, '\0', sizeof(pszBuffer));
        }
        shutdown(hClientSocket, SHUT_RDWR);
        close(hClientSocket);
    }
    close(hServerSocket);
    return 0;
}