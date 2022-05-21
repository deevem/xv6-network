#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fcntl.h"

void test2(void){
    char *argv[2];
    argv[0] = "cat";
    argv[1] = 0;
    if(fork() == 0) {
    close(0);
    open("input.txt", O_RDONLY|O_CREATE);
    exec("cat", argv);
    }
}

int main(void)
{
    test2();
    exit(0);
}