#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <memory.h>
#include <arpa/inet.h>
// DNS 를 이용하기 위한 Header
#include <netdb.h>


void ErrorHandler(const char *msg);

int main(int argc, char **argv) {
    /** DNS 에서 가져온 정보를 저장하기 위한 구조체 */
    struct hostent *host;
    struct in_addr addr;


    char buf[128] = "www.naver.com";
    /** 해당 도메인 주소를 바탕으로 IP 주소 값을 가져오는 함수 */
    host = gethostbyname("www.naver.com");
    if (!host) {
        ErrorHandler("gethostbyname fail");
    }

    printf("호스트 이름 : %s\n", host->h_name);
    printf("호스트 주소타입 번호 : %d\n", host->h_addrtype);
    printf("호스트 주소의 길이 : %d\n", host->h_length);
    for (int i = 0; host->h_addr_list[i]; i++) {
        memcpy(&addr.s_addr, host->h_addr_list[i], sizeof(addr.s_addr));
        inet_ntop(AF_INET, &addr, buf, sizeof(buf));
        printf("IP 주소(%d 번째) : %s\n", i + 1, buf);
    }
    for (int i = 0; host->h_aliases[i]; i++) {
        printf("호스트 별명(%d 번째) : %s\n", i + 1, host->h_aliases[i]);
    }
    return 0;
}

void ErrorHandler(const char *msg) {
    fputs(msg, stderr);
    fputc('\r\n', stderr);
    fflush(stderr);
    exit(1);
}