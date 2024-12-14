#include <stdio.h>
#include <pthread.h>

void* thread_fun(void* arg) {
    printf("线程 %ld 运行\n", (long)arg);
    return NULL;
}

int main() {
    pthread_t myThread1, myThread2;

    if (pthread_create(&myThread1, NULL, thread_fun, (void*)1) != 0) {
        perror("创建线程1失败！");
        return 1;
    }
    if (pthread_create(&myThread2, NULL, thread_fun, (void*)2) != 0) {
        perror("创建线程2失败！");
        return 1;
    }
    pthread_join(myThread1, NULL);
    pthread_join(myThread2, NULL);

    return 0;
}