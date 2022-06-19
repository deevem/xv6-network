#include "../kernel/types.h"
#include "../kernel/stat.h"
#include "../kernel/param.h"
#include "user.h"

#define MAKE_IP_ADDR(a, b, c, d) (((uint32_t)a << 24) | ((uint32_t)b << 16) | ((uint32_t)c << 8) | (uint32_t)d)

int main(){
    int fd = connect(MAKE_IP_ADDR(183,172,152,232), 2000, 54321);
    printf("create success\n");
    char tmp[128];
    memset(&tmp, 0, sizeof(tmp));
    int c = read(fd, tmp, sizeof(tmp) - 1);
    if (c < 0)
    {
        printf("recvudp failed\n");
    }
    printf("recv udp\n");
    printf("%s\n", tmp);
    exit(0);
}