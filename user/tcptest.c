#include "../kernel/types.h"
#include "../kernel/stat.h"
#include "../kernel/param.h"
#include "user.h"

#define MAKE_IP_ADDR(a, b, c, d) (((uint32_t)a << 24) | ((uint32_t)b << 16) | ((uint32_t)c << 8) | (uint32_t)d)

void asClient() {
    int fd;
    char message[] = "test message form xv6\0";
    if ((fd = connect_tcp(MAKE_IP_ADDR(127,0,0,1), 2333)) < 0) {
        printf("tcp GG\n");
    }
    printf("connect finish\n");
    write(fd, (const void *)message, strlen(message));
    printf("write finish\n");
    close(fd);
    exit(0);
}

void asServer() {
    int fd;
    if ((fd = bind_tcp(2222)) < 0) {
        printf("tcp bind fail \n");
    }

    if (listen_tcp(fd, 5) < 0) {
        printf("tcp listen fail");
    }

    printf("listen finished\n");

    int nfd = accept_tcp(fd);

    printf("accept %d!\n", nfd);

    close(nfd);

    close(fd);
}

int main() {
    asClient();
    exit(0);
    return 0;
}