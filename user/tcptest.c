#include "../kernel/types.h"
#include "../kernel/stat.h"
#include "../kernel/param.h"
#include "user.h"

#define MAKE_IP_ADDR(a, b, c, d) (((uint32_t)a << 24) | ((uint32_t)b << 16) | ((uint32_t)c << 8) | (uint32_t)d)

int main() {
    int fd;
    if ((fd = connect_tcp(MAKE_IP_ADDR(182,61,200,7), 80, 34)) < 0) {
        printf("tcp GG\n");
    }
    exit(0);
}