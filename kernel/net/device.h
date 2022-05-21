#ifndef DEVICE
#define DEVICE

#include "sysheaders.h"

int net_read(char *buf, int len);
int net_write(char* buf, int len);
void net_init();

#endif