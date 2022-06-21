#include "../kernel/types.h"
#include "../kernel/stat.h"
#include "../kernel/param.h"
#include "user.h"

#define MAKE_IP_ADDR(a, b, c, d) (((uint32_t)a << 24) | ((uint32_t)b << 16) | ((uint32_t)c << 8) | (uint32_t)d)

void asClient() {
    int fd;
    if ((fd = connect_tcp(MAKE_IP_ADDR(182,61,200,7), 80, 34)) < 0) {
        printf("tcp GG\n");
    }
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
    asServer();
    exit(0);
    return 0;
}