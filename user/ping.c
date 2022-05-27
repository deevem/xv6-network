#include "../kernel/types.h"
#include "../kernel/stat.h"
#include "../kernel/param.h"
#include "user.h"

uint32 judge_ip(char *s) {
    int i = 0;
    int v1 = 0, v2 = 0, v3 = 0, v4 = 0;
    for (i = 0; s[i] != '\0'; ++i)
        ;
    for (i = 0; s[i] != '.'; ++i){
        if(s[i] > '9' || s[i] < '0') return 0;
        v1 = v1 * 10 + s[i] - '0';
    }
    if(v1 < 0 || v1 > 255) return 0;
    for (++i; s[i] != '.'; ++i){
        if(s[i] > '9' || s[i] < '0') return 0;
        v2 = v2 * 10 + s[i] - '0';
    }
    if(v2 < 0 || v4 > 255) return 0;

    for (++i; s[i] != '.'; ++i){
        if(s[i] > '9' || s[i] < '0') return 0;
        v3 = v3 * 10 + s[i] - '0';
    }
    if(v3 < 0 || v4 > 255) return 0;

    for (++i; s[i] != '\0'; ++i){
        if(s[i] > '9' || s[i] < '0') return 0;
        v4 = v4 * 10 + s[i] - '0';
    }
    if(v4 < 0 || v4 > 255) return 0;
    return 1;
}

uint32 ip2int(char *sIP)
{
    int i = 0;
    uint32 v1 = 0, v2 = 0, v3 = 0, v4 = 0;
    for (i = 0; sIP[i] != '\0'; ++i)
        ;
    for (i = 0; sIP[i] != '.'; ++i)
        v1 = v1 * 10 + sIP[i] - '0';
    for (++i; sIP[i] != '.'; ++i)
        v2 = v2 * 10 + sIP[i] - '0';
    for (++i; sIP[i] != '.'; ++i)
        v3 = v3 * 10 + sIP[i] - '0';
    for (++i; sIP[i] != '\0'; ++i)
        v4 = v4 * 10 + sIP[i] - '0';
    return (v1 << 24) + (v2 << 16) + (v3 << 8) + v4;
}

int main(int argc, char **argv)
{
    if (argc == 1)
    {
        printf("Usage: ping target_name \n");
        return 0;
    }

    int fd;


    uint32 dst;
    
    if(judge_ip(argv[1])){ // ipp address
        dst = ip2int(argv[1]);
    }
    else {
        dst = dns(argv[1]);
    }
    
    int id = getpid();
    int icmp_seq = 0, attempts = 4, send_ok = 4, recv_ok = 4;
    uint32 start_time = 0, end_time = 0;


    if ((fd = connect_icmp(dst, 8, 0)) < 0)
    {
        printf("ping: connect() failed\n");
        return -1;
    }

    char ibuf[128];
    memset(&ibuf, 0, sizeof(ibuf));
    printf("Ping %s ...\n", argv[1]);
    printf("    IP address is:%d.%d.%d.%d\n",(dst >> 24) & 0xff,(dst >> 16) & 0xff,(dst >> 8) & 0xff,dst & 0xff);
    for (int i = 0; i < attempts; i++)
    {
        sleep(50);
        uint16 header[3];
        header[0] = 0x0800; // icmp type
        header[1] = id; // icmp code
        header[2] = icmp_seq;
        start_time = uptime();
        if (write(fd, header, sizeof(header)) < 0)
        {
            printf("ping: send() failed\n");
            send_ok -= 1;
            continue;
        }

        int cc = read(fd, ibuf, sizeof(ibuf)-1);
        end_time = uptime();
        icmp_seq += 1;
        printf("From %s: icmp_seq=%d time=%dms\n", argv[1], icmp_seq , (end_time - start_time) * 10);
        if (cc < 0)
        {
            printf("ping: recv() failed\n");
            recv_ok -= 1;
            continue;
        }
    }
    close(fd);
    printf("%s ping statics:\n", argv[1]);
    printf("%d packets transmittedd, %d received, %d% packet loss\n", send_ok, recv_ok, (4 - recv_ok) * 100 / send_ok);
    exit(0);
}