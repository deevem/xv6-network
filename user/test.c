#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void test(void){
    int pid;
    pid = fork();
    if(pid > 0){
        printf("parent: child=%d\n", pid);
        pid = wait((int*)0);
        printf("child %d is done\n", pid);
    } else if(pid == 0){
        printf("child: exiting\n");
        exit(0);
    } else {
        printf("fork error\n");
    }
}

int main(void)
{
    test();
    exit(0);
}