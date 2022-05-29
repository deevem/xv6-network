#include "dns.h"
#include "user.h"
int main(int argc, char **argv)
{
    int res = dns("baidu.com");
    printf("%d\n", res);
    printf("end of nslookup\n");
    exit(0);
    return 0;
}