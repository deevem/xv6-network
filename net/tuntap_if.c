#include "tuntap_if.h"
#include <net/if.h>

int tun_fd;

// if == interface
// open tuntap
static int tun_alloc(char *dev, int flags)
{
struct ifreq ifr;
    int fd, err;

    if( (fd = open("/dev/net/tun", O_RDWR)) < 0 ) {
        perror("Cannot open TUN/TAP dev\n"
                    "Make sure one exists with " 
                    "'$ mknod /dev/net/tap c 10 200'");
        exit(1);
    }


    ifr.ifr_flags = flags;
    if( *dev ) {
        strncpy(ifr.ifr_name, dev, IFNAMSIZ);
    }

    if( (err = ioctl(fd, TUNSETIFF, (void *) &ifr)) < 0 ){
        perror("ERR: Could not ioctl tun");
        close(fd);
        return err;
    }

    strcpy(dev, ifr.ifr_name);
    return fd;
}

int tun_read(char *buf, int len) {
    return read(tun_fd, buf, len);
}

int tun_write(char* buf, int len) {
    return write(tun_fd, buf, len);
}

void tun_init() {
    char tun_name[IFNAMSIZ];
    tun_name[0] = '\0';
    tun_fd = tun_alloc(tun_name, IFF_TUN | IFF_NO_PI);
    printf("Init %s\n", tun_name);
    if (strcmp(tun_name, "tun0") == 0) {
        system("sudo ip l s tun0 up");
        // system("sudo ip route add dev tun0 10.0.0.0/24");
        system("sudo ip a a 10.1.1.2/24 dev tun0");
    }
    if (strcmp(tun_name, "tun1") == 0) {
        system("sudo ip l s tun1 up");
        // system("sudo ip route add dev tun0 10.0.0.0/24");
        system("sudo ip a a 10.0.0.2/24 dev tun1");
    }
}

void tap_init() {
    char tun_name[IFNAMSIZ];
    tun_name[0] = '\0';
    tun_fd = tun_alloc(tun_name, IFF_TAP | IFF_NO_PI);
    printf("Init %s\n", tun_name);
    if (strcmp(tun_name, "tap0") == 0) {
        system("sudo ip l s tap0 up");
        system("sudo ip a a 10.1.1.2/24 dev tap0");
    }
    if (strcmp(tun_name, "tap1") == 0) {
        system("sudo ip l s tap1 up");
        system("sudo ip a a 10.0.0.2/24 dev tap1");
    }
}
