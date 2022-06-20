#include "../kernel/types.h"
#include "../kernel/stat.h"
#include "../kernel/param.h"
#include "user.h"

#define MAKE_IP_ADDR(a, b, c, d) (((uint32_t)a << 24) | ((uint32_t)b << 16) | ((uint32_t)c << 8) | (uint32_t)d)

int main() {
    
    char output[] = "test udp socket\0";
    //char input[1000];
    
    int fd = connect(MAKE_IP_ADDR(SERVER_IP1, SERVER_IP2, SERVER_IP3, SERVER_IP4), 12345, 54321);
    if (write(fd, output, strlen(output)) < 0) {
        printf("udp tx failed\n");
        exit(1);
    } else {
        printf("packet sent\n");
    }
    exit(0);
}