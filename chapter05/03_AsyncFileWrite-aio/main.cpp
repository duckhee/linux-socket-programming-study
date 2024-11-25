#include <iostream>
#include <aio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/syscall.h>
#include <pthread.h>
#include <memory.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <csignal>

#if defined(__APPLE__)
typedef sigval sigval_t;
#endif

void *ThreadFunction(void *pParam);

void AsyncComplete(sigval_t sig);

void AsyncSignalDo(int);

struct aiocb *g_aioList[1];

pthread_attr_t g_thread_attr;

int main(int argc, char **argv) {
    /** Thread 생성 정보를 담기 위한 구조체 */
    pthread_t thread;
    FILE *pFile = fopen("test.txt", "wb");
    int fp = fileno(pFile);
    /** set signal detected */
    signal(SIGUSR1, AsyncSignalDo);
    pthread_create(&thread, NULL, ThreadFunction, &fp);
    pthread_detach(thread);
//    pthread_join(thread, nullptr);
    sleep(600);

    aio_suspend(g_aioList, 1, nullptr);

    return 0;
}

void *ThreadFunction(void *pParam) {
    std::cout << "Call Thread" << std::endl;
    int fp = (*(int *) pParam);
    char *pszBuffer = new char[16];
    memset(pszBuffer, '\0', sizeof(char) * 16);
    strcpy(pszBuffer, "Hello Async");
    /** 비동기 요청을 위한 구조체 */
    aiocb my_aio = {0,};
    memset(&my_aio, '\0', sizeof(my_aio));
    my_aio.aio_fildes = fp;
//    my_aio.aio_offset = 1024 * 1024 * 32; // 32MB Offset
    my_aio.aio_offset = 1024 * 1024 * 2; // 2MB Offset
    my_aio.aio_buf = pszBuffer;
    my_aio.aio_nbytes = sizeof(char) * 16;
//    my_aio.aio_sigevent.sigev_notify = SIGEV_NONE;
//    my_aio.aio_sigevent.sigev_notify = SIGEV_THREAD;
    my_aio.aio_sigevent.sigev_signo = SIGUSR1;
    my_aio.aio_sigevent.sigev_notify = SIGEV_SIGNAL;
//    my_aio.aio_sigevent.sigev_signo = SIGUSR2;
//    my_aio.aio_sigevent.sigev_value = ;
//    my_aio.aio_lio_opcode = LIO_WRITE;
    my_aio.aio_sigevent.sigev_notify_function = AsyncComplete;
    my_aio.aio_sigevent.sigev_value.sival_ptr = &my_aio;
    g_aioList[0] = (struct aiocb *) malloc(sizeof(struct aiocb));
    g_aioList[0] = {&my_aio};
//    int test = lio_listio(LIO_WAIT, g_aioList, 1, &my_aio.aio_sigevent);
    int ret = aio_write(&my_aio);
//    aio_cancel(fp, &my_aio);
    printf("aio_write : %d\r\n", ret);
//    int result = aio_suspend(g_aioList, 1, nullptr);

//    printf("Done : %d\r\n", result);

    return (void *) 0;
}

void AsyncComplete(union sigval sigval) {
    printf("AsyncComplete");
    struct aiocb *req;
    req = (struct aiocb *) &sigval.sival_ptr; //Pay attention here.
//    printf("Async File Write Done : %d\r\n", req->aio_fildes);
/* Did the request complete? */
    if (aio_error(req) == 0) {

        /* Request completed successfully, get the return status */

//        printf("buff[%s]\n", req->aio_buf);
//        delete[] req->aio_buf;
        close(req->aio_fildes);
    }
}

void AsyncSignalDo(int sig) {

    printf("Test SIGINT Call!\r\n");
    printf("AsyncSignalDo : %s\r\n", (sig == SIGUSR1) ? "SIGUSR1" : "no");
    union sigval *signalValue = (union sigval *) &sig;
//    printf("AsyncSignalDo sigval : %s\r\n", (char *) signalValue->sival_ptr);
    struct aiocb *req;
//    req = (struct aiocb *) signalValue->sival_ptr; //Pay attention here.
    req = g_aioList[0];
//    printf("Async File Write Done : %d\r\n", req->aio_fildes);
/* Did the request complete? */
    if (aio_error(req) == 0) {

        /* Request completed successfully, get the return status */
        ssize_t ret = aio_return(req);
        printf("ret [%ld]\n", ret);

//        printf("fp : %d\r\n", req->aio_fildes ? -1 : req->aio_fildes);
//        printf("buff[%s]\n", (req->aio_buf == nullptr) ? "not have buffer" : (char *) req->aio_buf);
//        delete[] req->aio_buf;
//        close(req->aio_fildes);
    } else {
        printf("AsyncSignalDo Error code : %d\r\n", aio_error(req));
    }
}