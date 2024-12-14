#include <stdio.h>  
#include <unistd.h>  
#include <sys/types.h>  

int main() {  
    pid_t pid1, pid2;  
    
    pid1 = fork();  
    
    if (pid1 < 0) {  
        fprintf(stderr, "失败！");  
        return 1;  
    }  
    else if (pid1 == 0) {  
        printf("子进程 1 (PID: %d, 父进程 PID： %d) 输出： b\n",   
               getpid(), getppid());  
    }  
    else {  
        pid2 = fork();  
        
        if (pid2 < 0) {  
            fprintf(stderr, "失败！");  
            return 1;  
        }  
        else if (pid2 == 0) {  
            printf("子进程 2 (PID: %d, 父进程 PID： %d) 输出： c\n",   
                   getpid(), getppid());  
        }  
        else {   
            printf("父进程 (PID: %d) 输出： a\n", getpid());  
        }  
    }  
    
    return 0;  
}  