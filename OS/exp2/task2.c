#include <stdio.h>  
#include <unistd.h>  
#include <sys/types.h>  

#define COUNT 1000
int main() {  
    pid_t pid1, pid2;  
    int i;     
    pid1 = fork();  
    
    if (pid1 < 0) {  
        fprintf(stderr, "失败！");  
        return 1;  
    }  
    else if (pid1 == 0) {   
        for (i = 0; i < COUNT; i++) {  
            printf("b");  
            fflush(stdout);  
        }  
    }  
    else {  
        pid2 = fork();  
        if (pid2 < 0) {  
            fprintf(stderr, "失败！");  
            return 1;  
        }  
        else if (pid2 == 0) {  
            for (i = 0; i < COUNT; i++) {  
                printf("c");  
                fflush(stdout);  
            }  
        }  
        else {  
            for (i = 0; i < COUNT; i++) {  
                printf("a");  
                fflush(stdout);  
            }  
        }  
    }  
    return 0;  
}  