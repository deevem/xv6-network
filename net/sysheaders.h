// This header file stores many useful and often used system headers, all commented

// uint types
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

#include <stdio.h> // perror
// open
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <unistd.h> // close()

#include <stdlib.h> // exit
#include <string.h> // memset
#include <sys/ioctl.h> // ioctl

#include <net/if.h> // ifreq
#include <linux/if_tun.h> // IFF_TAP , IFF_NO_PI
