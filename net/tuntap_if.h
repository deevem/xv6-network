#include "sysheaders.h"

static int tun_alloc(char* dev);
int tun_read(char *buf, int len);
int tun_write(char* buf, int len);
void tun_init();