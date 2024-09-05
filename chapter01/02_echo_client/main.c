#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>


#define BUFFER_MAX          128


int main(int argc, char **argv) {
    int fSocket;
    struct sockaddr_in serverAddr = {0,};

    char pszBuffer[BUFFER_MAX] = {0,};
    serverAddr.sin_family = PF_INET;
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverAddr.sin_port = htons(25000);
    /** client socket 생성 */
    fSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (fSocket == -1) {
        puts("ERROR: Create Client socket Failed...");
        return 0;
    }

    int isConnected = connect(fSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));

    if (isConnected == -1) {
        puts("ERROR: Failed Connected server");
        return 0;
    }

    while (true) {
        scanf("%s", pszBuffer);
        if (strcmp(pszBuffer, "EXIT") == 0) {
            break;
        }
        send(fSocket, pszBuffer, sizeof(pszBuffer) + 1, 0);
        memset(pszBuffer, '\0', sizeof(pszBuffer));
        recv(fSocket, pszBuffer, sizeof(pszBuffer), 0);
        printf("From Server : %s\r\n", pszBuffer);
    }
    shutdown(fSocket, SHUT_RDWR);

    close(fSocket);
    return 0;
}