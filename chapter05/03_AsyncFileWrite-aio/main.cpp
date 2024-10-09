#include <iostream>
#include <aio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/syscall.h>
#include <pthread.h>
#include <memory.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>

#if defined(__APPLE__)
typedef sigval sigval_t;
#endif

void *ThreadFunction(void *pParam);

void AsyncComplete(sigval_t sig);

int main(int argc, char **argv) {
    /** Thread 생성 정보를 담기 위한 구조체 */
    pthread_t thread;
    FILE *pFile = fopen("test.txt", "wb");
    int fp = fileno(pFile);


    pthread_create(&thread, NULL, ThreadFunction, &fp);
    pthread_join(thread, 0);
    return 0;
}

void *ThreadFunction(void *pParam) {
    std::cout << "Call Thread" << std::endl;
    int fp = (*(int *) pParam);
    char *pszBuffer = new char[16];
    memset(pszBuffer, '\0', sizeof(char) * 16);
    strcpy(pszBuffer, "Hello Async");
    /** 비동기 요청을 위한 구조체 */
    aiocb my_aio;
    memset(&my_aio, '\0', sizeof(my_aio));
    my_aio.aio_fildes = fp;
//    my_aio.aio_offset = 1024 * 1024 * 32; // 32MB Offset
    my_aio.aio_offset = 1024 * 1024 * 2; // 2MB Offset
    my_aio.aio_buf = pszBuffer;
    my_aio.aio_nbytes = sizeof(char) * 16;
    my_aio.aio_sigevent.sigev_notify = SIGEV_THREAD;
    my_aio.aio_sigevent.sigev_notify_function = &AsyncComplete;
    my_aio.aio_sigevent.sigev_notify_attributes = NULL;
    my_aio.aio_sigevent.sigev_value.sival_ptr = &my_aio;

    const struct aiocb *const aioRequest = &my_aio;

    aio_write(&my_aio);
    aio_suspend(&aioRequest, 1, NULL);
    printf("Done\r\n");

    return 0;
}

void AsyncComplete(sigval_t sigval) {
    printf("AsyncComplete");

    struct aiocb *req;
    req = (struct aiocb *) sigval.sival_ptr; //Pay attention here.
//    printf("Async File Write Done : %d\r\n", req->aio_fildes);
/* Did the request complete? */
    if (aio_error(req) == 0) {

        /* Request completed successfully, get the return status */
        int ret = aio_return(req);
        printf("ret [%d]\n", ret);
        printf("buff[%s]\n", req->aio_buf);
        delete[] req->aio_buf;
        close(req->aio_fildes);
    }
}