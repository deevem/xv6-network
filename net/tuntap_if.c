#include "tuntap_if.h"

static int tun_fd;
static char* dev;

char *tapaddr = "10.0.0.5";
char *taproute = "10.0.0.0/24";

// if == interface
// open tuntap
static int tun_alloc(char* dev) {
    struct ifreq ifr;
    int fd, err;

    if( (fd = open("/dev/net/tun", O_RDWR)) < 0 ) {
        perror("Cannot open TUN/TAP dev\n"
                    "Make sure one exists with " 
                    "'$ mknod /dev/net/tap c 10 200'");
        exit(1);
    }

    memset(&(ifr), 0, sizeof(ifr));

    ifr.ifr_flags = IFF_TUN | IFF_NO_PI;

    if (*dev) {
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
    dev = calloc(10, 1);
    tun_fd = tun_alloc(dev);
    
    // if (set_if_up(dev) != 0) {
    //     print_err("ERROR when setting up if\n");
    // }

    // if (set_if_route(dev, taproute) != 0) {
    //     print_err("ERROR when setting route for if\n");
    // }

    // if (set_if_address(dev, tapaddr) != 0) {
    //     print_err("ERROR when setting addr for if\n");
    // }
}

// int main() {
//     int nread;
//     char buffer[4096];
//     char tun_name[IFNAMSIZ];
//     tun_init();

//     while (1) {
//         unsigned char ip[4];
//         nread = tun_read(buffer, sizeof(buffer));
//         if (nread < 0) {
//             perror("Reading from interface error");
//             close(tun_fd);
//             exit(1);
//         }

//         printf("Read %d bytes from tun/tap device\n", nread);

//         memcpy(ip, &buffer[12], 4);
//         memcpy(&buffer[12], &buffer[16], 4);
//         memcpy(&buffer[16], ip, 4);

//         buffer[20] = 0;
//         *((unsigned short *)&buffer[22]) += 8;
    
//         nread = tun_write(buffer, nread);

//         printf("Write %d bytes to tun/tap device, that's %s\n", nread, buffer);

//     }
    
// }