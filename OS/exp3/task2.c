#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void* thread_fun(void* arg) {
    int* args = (int*)arg;
    printf("接收到字符 %c 和整型 %d\n", args[0], args[1]);
    free(args);
    return NULL;
}

int main() {
    pthread_t thread;
    int* args = malloc(2 * sizeof(int));
    args[0] = 'A'; 
    args[1] = 42;  

    if (pthread_create(&thread, NULL, thread_fun, (void*)args) != 0) {
        perror("线程创建失败！");
        return 1;
    }

    pthread_join(thread, NULL);

    return 0;
}