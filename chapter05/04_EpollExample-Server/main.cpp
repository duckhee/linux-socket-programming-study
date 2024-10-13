#include <iostream>
#include <sys/socket.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <sys/errno.h>
#include <sys/types.h>
//#include <sys/event.h>
#include <sys/time.h>

void ErrorHandle(const char *msg);

int main(int argc, char **argv) {
    return 0;
}


void ErrorHandle(const char *msg) {
    fputs(msg, stderr);
    fputc('\r\n', stderr);
    exit(1);
}
