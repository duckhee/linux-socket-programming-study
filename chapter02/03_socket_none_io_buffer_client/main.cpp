#include <iostream>
#include <unistd.h>
#include <memory.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <string.h>
#include <errno.h>


using namespace std;

int main(int argc, char **argv) {
    int clientSocket;
    sockaddr_in serverInfo = {0,};

    clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (clientSocket == -1) {
        cout << "Failed Create socket ..." << endl;
        return -1;
    }

    serverInfo.sin_family = PF_INET;
    serverInfo.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverInfo.sin_port = htons(25000);

    if (connect(clientSocket, (sockaddr *) &serverInfo, sizeof(serverInfo)) == -1) {
        cout << "Failed connect server ..." << endl;
        close(clientSocket);
        return -1;
    }
    int nOpt = 1;
    /** client socket setting TCP No delay -> not using IO Buffer */
    setsockopt(
            clientSocket,
            IPPROTO_TCP,
            TCP_NODELAY,
            (void *) &nOpt,
            sizeof(nOpt)
    );


    char pszBuffer[128] = {0,};

    while (true) {
        fgets(pszBuffer, 128, stdin);
        if (strcmp(pszBuffer, "EXIT\n") == 0) {
            break;
        }
        int pszLength = strlen(pszBuffer);
//        cout << "Buffer Length : " << pszLength << endl;
        for (int i = 0; i < pszLength; ++i) {
            send(clientSocket, pszBuffer + i, 1, 0);
//            write(clientSocket, (pszBuffer + i), 1);
        }

        memset(pszBuffer, '\0', sizeof(pszBuffer));
//        recv(clientSocket, pszBuffer, 128, 0);
        read(clientSocket, pszBuffer, 128);
        printf("From Server : %s\r\n", pszBuffer);
    }
    shutdown(clientSocket, SHUT_RDWR);
    close(clientSocket);
    return 0;
}
