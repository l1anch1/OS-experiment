#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>

#define MSG_SIZE 50

int main() {
    int fd[2];  // fd[0]用于读取，fd[1]用于写入
    pid_t pid1, pid2;
    char msg1[] = "Child 1 is sending a message!";
    char msg2[] = "Child 2 is sending a message!";
    char buffer[MSG_SIZE];
    
    if (pipe(fd) == -1) {
        fprintf(stderr, "创建管道失败！");
        return 1;
    }
    
    // Create first child process
    pid1 = fork();
    
    if (pid1 < 0) {
        fprintf(stderr, "子进程1创建失败！");
        return 1;
    }
    else if (pid1 == 0) {
        close(fd[0]);  
        write(fd[1], msg1, strlen(msg1) + 1);
        close(fd[1]);
        return 0;
    }
    else {
        pid2 = fork();
        
        if (pid2 < 0) {
            fprintf(stderr, "子进程2创建失败！");
            return 1;
        }
        else if (pid2 == 0) {
            // Second child process
            close(fd[0]);  // Close reading end
            write(fd[1], msg2, strlen(msg2) + 1);
            close(fd[1]);
            return 0;
        }
        else {
            // Parent process
            close(fd[1]);  // Close writing end
            
            // Read first message
            read(fd[0], buffer, MSG_SIZE);
            printf("Parent received: %s\n", buffer);
            
            // Read second message
            read(fd[0], buffer, MSG_SIZE);
            printf("Parent received: %s\n", buffer);
            
            close(fd[0]);
        }
    }
    
    return 0;
}