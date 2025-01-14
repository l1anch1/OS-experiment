#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>  
#include <sys/ipc.h>  
#include <sys/msg.h>  
#include <sys/wait.h>  
#include <unistd.h>  

#define MSG_KEY 1234  
#define MSG_SIZE 100  
#define MSG_NORMAL 1  
#define MSG_EXIT 2  

// 消息结构体  
typedef struct {  
    long mtype;  
    char mtext[MSG_SIZE];  
} Message;  

// 错误处理函数  
void handle_error(const char *msg) {  
    perror(msg);  
    exit(EXIT_FAILURE);  
}  

// 子进程处理函数  
void child_process(int msgid) {  
    Message msg;  
    char buffer[MSG_SIZE];  

    while (1) {  
        printf("Enter message (type 'exit' to quit): ");  
        fgets(buffer, sizeof(buffer), stdin);  
        
        if (strncmp(buffer, "exit", 4) == 0) {  
            break;  
        }  

        msg.mtype = MSG_NORMAL;  
        strncpy(msg.mtext, buffer, sizeof(msg.mtext) - 1);  
        
        if (msgsnd(msgid, &msg, sizeof(msg.mtext), 0) == -1) {  
            handle_error("msgsnd error");  
        }  
    }  

    // 发送退出消息  
    msg.mtype = MSG_EXIT;  
    msgsnd(msgid, &msg, sizeof(msg.mtext), 0);  
    exit(EXIT_SUCCESS);  
}  

// 父进程处理函数  
void parent_process(int msgid) {  
    Message msg;  

    while (1) {  
        if (msgrcv(msgid, &msg, sizeof(msg.mtext), MSG_NORMAL, 0) == -1) {  
            handle_error("msgrcv error");  
        }  

        printf("Received message: %s", msg.mtext);  

        if (msg.mtype == MSG_EXIT) {  
            break;  
        }  
    }  

    // 清理消息队列  
    msgctl(msgid, IPC_RMID, NULL);  
    wait(NULL);  
    exit(EXIT_SUCCESS);  
}  

int main() {  
    // 创建消息队列  
    int msgid = msgget((key_t)MSG_KEY, 0666 | IPC_CREAT);  
    if (msgid == -1) {  
        handle_error("msgget error");  
    }  

    // 创建子进程  
    pid_t pid = fork();  
    if (pid == -1) {  
        handle_error("fork error");  
    }  

    // 根据进程ID执行相应功能  
    if (pid == 0) {  
        child_process(msgid);  
    } else {  
        parent_process(msgid);  
    }  

    return 0;  
}