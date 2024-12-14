#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

void sigint_handler(int signo) {
    if(signo == SIGINT)
        printf("Received SIGINT (signal number: %d) in process %d\n", signo, getpid());
    else{
        printf("received signal %d\n", signo);
        exit(1);
    }
}

void sigusr1_handler(int signo) {
    if(signo == SIGUSR1)
        printf("Received SIGUSR1 (signal number: %d) in process %d\n", signo, getpid());
    else{
        printf("received signal %d\n", signo);
        exit(1);
    }
}

int main() {
    pid_t pid;

    // 注册信号处理程序
    if (signal(SIGINT, sigint_handler) == SIG_ERR) {
        perror("Cannot handle SIGINT");
        exit(1);
    }

    if (signal(SIGUSR1, sigusr1_handler) == SIG_ERR) {
        perror("Cannot handle SIGUSR1");
        exit(1);
    }

    pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        exit(1);
    } else if (pid > 0) {
        printf("Parent process PID: %d\n", getpid());
        pause();  
    } else {
        printf("Child process PID: %d\n", getpid());
        pause();
    }

    return 0;
}