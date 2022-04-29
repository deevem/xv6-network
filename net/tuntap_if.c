#include "tuntap_if.h"
#include <net/if.h>

int tun_fd;

// if == interface
// open tuntap
static int tun_alloc(char *dev, int flags)
{
    struct ifreq ifr;
    int fd, err;

    char *clonedev = "/dev/net/tun";

    if ((fd = open(clonedev, O_RDWR)) < 0) {
        return fd;
    }

    memset(&ifr, 0, sizeof(ifr));
    ifr.ifr_flags = flags;
    
    if (*dev != '\0') {
        strncpy(ifr.ifr_name, dev, IFNAMSIZ);
    }
    if ((err = ioctl(fd, TUNSETIFF, (void *) &ifr)) < 0) {
        close(fd);
        return err;
    }

    // 一旦设备开启成功，系统会给设备分配一个名称，对于tun设备，一般为tunX，X为从0开始的编号；
    // 对于tap设备，一般为tapX
    strcpy(dev, ifr.ifr_name);

    return fd;
}

int tun_read(char *buf, int len) {
    return read(tun_fd, buf, len);
}

int tun_write(char* buf, int len) {
    printf("%d \n", len);
    for (int i = 42; i < 72;i ++)
        printf("%c", buf[i]);
    printf("%d\n", tun_fd);
    return write(tun_fd, buf, len);
}

void tun_init() {
    char tun_name[IFNAMSIZ];
    tun_name[0] = '\0';
    tun_fd = tun_alloc(tun_name, IFF_TAP | IFF_NO_PI);
    printf("%s\n", tun_name);
}
