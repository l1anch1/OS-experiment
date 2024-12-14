#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define BUFFER_SIZE 2
#define NUM_DATA 10

int data[BUFFER_SIZE];
int temp = 0;

sem_t empty;
sem_t full;
sem_t mutex;
sem_t add;
sem_t mul;
sem_t read1;
sem_t read2;

const char *file1 = "../1.dat";
const char *file2 = "../2.dat";

void *read1_thread(void *arg) {
    FILE *file = (FILE *)arg;
    int value;
    while (fscanf(file, "%d", &value) == 1) {
        sem_wait(&read1);
        sem_wait(&empty);
        sem_wait(&mutex);

        data[0] = value;
        temp ++;

        sem_post(&mutex);
        sem_post(&read2);

        if(temp == 2)
                sem_post(&full);
    }
    return NULL;
}
void *read2_thread(void *arg) {
    FILE *file = (FILE *)arg;
    int value;
    while (fscanf(file, "%d", &value) == 1) {
        sem_wait(&read2);
        sem_wait(&empty);
        sem_wait(&mutex);

        data[1] = value;
        temp ++;

        sem_post(&mutex);
        sem_post(&read1);

        if(temp == 2)
                sem_post(&full);
    }
    return NULL;
}

void *operate1_thread(void *arg) {
    int* addition = (int*)arg;

    for (int i = 0; i < NUM_DATA; i++) {
        sem_wait(&add);
        sem_wait(&full);
        sem_wait(&mutex);

        int data1 = data[0];
        int data2 = data[1];

        int result = data1 + data2;
        char operator = '+';

        printf("%d %c %d = %d\n", data1, operator, data2, result);

        temp = 0;
        sem_post(&mutex);
        sem_post(&empty);
        sem_post(&empty);
        sem_post(&mul);
    }

    return NULL;
}
void *operate2_thread(void *arg) {
    int* addition = (int*)arg;

    for (int i = 0; i < NUM_DATA; i++) {
        sem_wait(&mul);
        sem_wait(&full);
        sem_wait(&mutex);


        int data1 = data[0];
        int data2 = data[1];

        int result = data1 * data2;
        char operator ='*';

        printf("%d %c %d = %d\n", data1, operator, data2, result);

        temp = 0;

        sem_post(&mutex);
        sem_post(&empty);
        sem_post(&empty);
        sem_post(&add);
    }

    return NULL;
}

int main() {
    FILE *file1_ptr = fopen(file1, "r");
    FILE *file2_ptr = fopen(file2, "r");
    if (!file1_ptr || !file2_ptr) {
     perror("Error opening files");
        return 1;
    }

    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);
    sem_init(&mutex, 0, 1);
    sem_init(&add, 0, 1);
    sem_init(&mul, 0, 0);
    sem_init(&read1, 0, 1);
    sem_init(&read2, 0, 0);

    pthread_t read1_tid, read2_tid, operate1_tid, operate2_tid;

    pthread_create(&read1_tid, NULL, read1_thread, file1_ptr);
    pthread_create(&read2_tid, NULL, read2_thread, file2_ptr);

    pthread_create(&operate1_tid, NULL, operate1_thread, (void *)1);  
    pthread_create(&operate2_tid, NULL, operate2_thread, (void *)0);  
    
    pthread_join(read1_tid, NULL);
    pthread_join(read2_tid, NULL);
    pthread_join(operate1_tid, NULL);
    pthread_join(operate2_tid, NULL);

    sem_destroy(&empty);
    sem_destroy(&full);
    sem_destroy(&mutex);

    fclose(file1_ptr);
    fclose(file2_ptr);

    return 0;
}