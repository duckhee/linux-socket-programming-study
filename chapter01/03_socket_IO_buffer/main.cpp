#include <iostream>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>


int main(int argc, char **argv) {
    /** socket 생성 -> TCP socket */
    int hSocket = socket(
            AF_INET,
            SOCK_STREAM,
            IPPROTO_TCP
    );
    /** socket 생성 실패 시 동잓 */
    if (hSocket == -1) {
        puts("ERROR: Failed Create Socket\r\n");
        return -1;
    }
    /** socket IO Buffer 를 확인하기 위한 변수 및 확인 flag 값 */
    int isSuccess = 0;
    int nBufferSize = 0;
    socklen_t nLength = sizeof(nBufferSize);
    /** socket에 적용이 도어 있는 전송 Buffer 크기 가져오기 */
    isSuccess = getsockopt(
            hSocket, // 가져올 소켓에 대한 File Descriptor
            SOL_SOCKET, // 어느 수준에서의 옵션 값을 가져올 것인지에 대한 설정
            SO_SNDBUF, // 가져올 옵션에 대한 지시자 -> Send Buffer에 대한 크기 가져오기 위한 SO_SNDBUF
            &nBufferSize, // 가져온 옵션에 대한 값을 담아줄 버퍼
            &nLength // 버퍼의 크기
    );
    /** 값을 가져오는 데 성공 시 출력 */
    if (isSuccess != -1) {
        printf("Get Send Buffer Size : %d\r\n", nBufferSize);
    }
    /** 옵션을 담아줄 변수들에 대한 초기화 */
    isSuccess = 0;
    nBufferSize = 0;
    nLength = sizeof(nBufferSize);
    /** socket에 적용이 도어 있는 전송 Buffer 크기 가져오기 */
    isSuccess = getsockopt(
            hSocket, // 가져올 소켓에 대한 File Descriptor
            SOL_SOCKET, // 어느 수준에서의 옵션 값을 가져올 것인지에 대한 설정
            SO_RCVBUF, // 가져올 옵션에 대한 지시자 -> Receive Buffer에 대한 크기를 가져오기 위한 SO_RCVBUF
            &nBufferSize, // 가져온 옵션에 대한 값을 담아줄 버퍼
            &nLength // 버퍼의 크기
    );
    /** 값을 가져오는 데 성공 시 출력 */
    if (isSuccess != -1) {
        printf("Get Receive Buffer Size : %d\r\n", nBufferSize);
    }

    /** close socket */
    close(hSocket);
    return 0;
}