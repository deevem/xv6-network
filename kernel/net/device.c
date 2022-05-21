#include "device.h"

int net_read(char *buf, int len) {
    printf("net device read\n");
    return 0;
}

int net_write(char* buf, int len) {
    printf("net device write\n");
    return 0;
}

void net_init() {
    printf("net device init\n");
}