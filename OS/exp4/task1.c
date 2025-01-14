#include <stdio.h>  
#include <stdlib.h>  
#include <pthread.h>  
#include <semaphore.h>  

#define BUFFER_SIZE 2  
#define NUM_DATA 10  

// 缓冲区数据结构  
typedef struct {  
    int data[BUFFER_SIZE];  
    int in;  
    int out;  
    int temp;  
} Buffer;  

Buffer buffer = {.in = 0, .out = 0, .temp = 0};  

sem_t empty;  
sem_t full;  
sem_t mutex;  

const char *file1 = "../1.dat";  
const char *file2 = "../2.dat";  

void *read_thread(void *arg) {  
    FILE *file = (FILE *)arg;  
    int value;  
    
    while (fscanf(file, "%d", &value) == 1) {  
        sem_wait(&empty);  
        sem_wait(&mutex);  

        buffer.data[buffer.in] = value;  
        buffer.in = (buffer.in + 1) % BUFFER_SIZE;  
        buffer.temp++;  

        sem_post(&mutex);  

        if(buffer.temp == 2)  
            sem_post(&full);  
    }  
    return NULL;  
}  

void *operate_thread(void *arg) {  
    int is_addition = (int)(long)arg;  
    
    for(int i = 0; i < NUM_DATA; i++) {  
        sem_wait(&full);  
        sem_wait(&mutex);  

        int data1 = buffer.data[0];  
        int data2 = buffer.data[1];  

        int result = is_addition ? data1 + data2 : data1 * data2;  
        char operator = is_addition ? '+' : '*';  

        printf("%d %c %d = %d\n", data1, operator, data2, result);  
        buffer.temp = 0;  

        sem_post(&mutex);  
        sem_post(&empty);  
        sem_post(&empty);  
    }  
    return NULL;  
}  

int main() {  
    // 打开文件  
    FILE *file1_ptr = fopen(file1, "r");  
    FILE *file2_ptr = fopen(file2, "r");  
    if (!file1_ptr || !file2_ptr) {  
        perror("Error opening files");  
        return 1;  
    }  

    // 初始化信号量  
    sem_init(&empty, 0, BUFFER_SIZE);  
    sem_init(&full, 0, 0);  
    sem_init(&mutex, 0, 1);  

    // 创建线程  
    pthread_t read1_tid, read2_tid, operate1_tid, operate2_tid;  
    pthread_create(&read1_tid, NULL, read_thread, file1_ptr);  
    pthread_create(&read2_tid, NULL, read_thread, file2_ptr);  
    pthread_create(&operate1_tid, NULL, operate_thread, (void *)1);  
    pthread_create(&operate2_tid, NULL, operate_thread, (void *)0);  

    // 等待线程结束  
    pthread_join(read1_tid, NULL);  
    pthread_join(read2_tid, NULL);  
    pthread_join(operate1_tid, NULL);  
    pthread_join(operate2_tid, NULL);  

    // 清理资源  
    sem_destroy(&empty);  
    sem_destroy(&full);  
    sem_destroy(&mutex);  
    fclose(file1_ptr);  
    fclose(file2_ptr);  

    return 0;  
}