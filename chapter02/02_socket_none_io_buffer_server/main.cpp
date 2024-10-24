#include <iostream>
#include <memory.h>
#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>

using namespace std;

int main(int argc, char **argv) {

    int serverSocket, clientSocket;
    /** server socket create */
    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (serverSocket == -1) {
        cout << "Failed Create socket..." << endl;
        return -1;
    }

    sockaddr_in serverInfo = {0,};
    serverInfo.sin_family = PF_INET;
    serverInfo.sin_addr.s_addr = htonl(INADDR_ANY);
    serverInfo.sin_port = htons(25000);

    int nOpt = 1;
    setsockopt(serverSocket, IPPROTO_TCP, TCP_NODELAY, &nOpt, sizeof(nOpt));

    if (::bind(serverSocket, (sockaddr *) &serverInfo, sizeof(serverInfo)) == -1) {
        cout << "Failed Socket Binding... " << endl;
        close(serverSocket);
        return -1;
    }

    if (listen(serverSocket, SOMAXCONN) == -1) {
        cout << "Failed Socket Listening..." << endl;
        close(serverSocket);
        return -1;
    }

    cout << "Server Started ..." << endl;
    sockaddr_in clientInfo = {0,};
    socklen_t clientInfoSize = sizeof(clientInfo);
    char pszBuffer[128] = {0,};
    int nReceiveSize = 0;

    while ((clientSocket = accept(serverSocket, (sockaddr *) &clientInfo, &clientInfoSize)) != -1) {
        cout << "Client Connected !" << endl;
        while ((nReceiveSize = recv(clientSocket, pszBuffer, 128, 0)) > 0) {
            send(clientSocket, pszBuffer, 128, 0);
//write(clientSocket, pszBuffer, 128);
            puts(pszBuffer);
            memset(pszBuffer, '\0', sizeof(pszBuffer));
        }

        shutdown(clientSocket, SHUT_RDWR);
        close(clientSocket);
        puts("Client Closed..");
    }

    close(serverSocket);
    return 0;
}
