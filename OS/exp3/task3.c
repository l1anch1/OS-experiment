#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define SIZE 1000

void* sort_function(void* arg) {
    int* data = (int*)arg;
    for (int i = 0; i < SIZE; i++) {
        for (int j = i + 1; j < SIZE; j++) {
            if (data[i] < data[j]) {
                int temp = data[i];
                data[i] = data[j];
                data[j] = temp;
            }
        }
    }
    printf("排序后的数组为 ");
    for (int i = 0; i < SIZE; i++) {
        printf("%d ",data[i]);
    } 
    return NULL;
}

void* sum_function(void* arg) {
    int* data = (int*)arg;
    int sum = 0;
    for (int i = 0; i < SIZE; i++) {
        sum += data[i];
    }
    printf("数组和为 %d\n", sum);
    return NULL;
}

int main() {
    pthread_t thread_sort, thread_sum;
    int data[SIZE];
    int value = 1;

    for (int i = 0; i < SIZE; i++) {
        data[i] = value++;
        if (value > 10) value = 1;
    }

    if (pthread_create(&thread_sort, NULL, sort_function, (void*)data) != 0) {
        perror("创建排序线程失败！");
        return 1;
    }
    if (pthread_create(&thread_sum, NULL, sum_function, (void*)data) != 0) {
        perror("创建求和线程失败！");
        return 1;
    }
    pthread_join(thread_sort, NULL);
    pthread_join(thread_sum, NULL);

    return 0;
}