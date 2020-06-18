#include "cliserv2.h"

int flags, i = 0;

void err_quit(const char*);

int main(int argc, char** argv)
{
    int fd, nloop, nusec;
    pid_t pid;
    char mesg[MESGSIZE];
    long offset;
    struct shmstruct* ptr;

    if (argc != 4) {
        fprintf(stderr, "Usage: %s <name> <#loops> <#usec>", argv[0]);
        exit(EXIT_FAILURE);
    }

    nloop = atoi(argv[2]);
    nusec = atoi(argv[3]);

    /* 用shm_open和mmap创建(服务器端创建的)共享存储区 */
    fd = shm_open(argv[1], O_RDWR, FILE_MODE);
    if (fd == -1) {
        err_quit("shm_open");
    }
    ptr = mmap(NULL, sizeof(struct shmstruct), PROT_READ | PROT_WRITE,
        MAP_SHARED, fd, 0);
    if (ptr == MAP_FAILED) {
        err_quit("mmap");
    }
    if (close(fd) == -1) {
        err_quit("close");
    }
    pid = getpid();
    /*
	在每次循环中,
	睡眠指定秒数
	用snprintf将pid和i打印到mesg
	调用sem_trywait对nempty进行P操作判断缓冲区是否已满
	如果返回值errno为EAGAIN,需要修改noverflow++(要考虑互斥),
	重新trywait
	如果缓冲区未满,对mutex调用sem_wait进入临界区
	获得msgdata数组偏移量offset
	修改nput(下一个可用slot)
	用strcpy拷贝mesg到msgdata
	注意利用信号量同步的顺序

	*/
    for (; i < nloop; ++i) {
        sleep(nusec);
        snprintf(mesg, MESGSIZE, "pid %d: message %d", pid, i);
        sem_trywait(&ptr->nempty);
        while (errno == EAGAIN) {
            sem_wait(&ptr->noverflowmutex);
            ptr->noverflow++;
            sem_post(&ptr->noverflowmutex);
            sem_trywait(&ptr->nempty);
        }
        sem_wait(&ptr->mutex);
        offset = ptr->msgoff[i];
        ptr->nput++;
        if(ptr->nput >= MESGSIZE)
            ptr->nput = 0;
        sem_post(&ptr->mutex);
        strcpy(&ptr->msgdata[offset], mesg);
        sem_post(&ptr->nstored);
    }
    exit(0);
}

void err_quit(const char* errmsg)
{
    /*fprintf(stderr, "%s\n", errmsg);*/
    perror(errmsg);
    exit(EXIT_FAILURE);
}
