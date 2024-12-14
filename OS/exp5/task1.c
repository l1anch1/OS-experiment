#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <unistd.h>


struct message {
    long mtype; 
    char mtext[100];
};

int main() {
    int msgid;
    struct message msg;
    pid_t pid;
    
    msgid = msgget((key_t)1234, 0666 | IPC_CREAT);
    if (msgid == -1) {
        perror("msgget error");
        exit(1);
    }

    pid = fork();
    if (pid == 0) {
        char buffer[100];
        while (1) {
            printf("Enter message (type 'exit' to quit): ");
            fgets(buffer, sizeof(buffer), stdin);
            if (strncmp(buffer, "exit", 4) == 0) {
                break;
            }
            msg.mtype = 1; // 设置消息类型
            strncpy(msg.mtext, buffer, sizeof(msg.mtext) - 1);
            if (msgsnd(msgid, &msg, sizeof(msg.mtext), 0) == -1) {
                perror("msgsnd error");
                exit(1);
            }
        }
        msg.mtype = 2; // 发送退出消息
        msgsnd(msgid, &msg, sizeof(msg.mtext), 0);
        exit(0);
    } else if (pid > 0) {
        while (1) {
            if (msgrcv(msgid, &msg, sizeof(msg.mtext), 1, 0) == -1) {
                perror("msgrcv error");
                exit(1);
            }
            printf("Received message: %s\n", msg.mtext);
            if (msg.mtype == 2) { // 接收到退出消息
                break;
            }
        }
        msgctl(msgid, IPC_RMID, NULL);
        wait(NULL); 
        exit(0);
    } else {
        perror("fork error");
        exit(1);
    }
    return 0;
}