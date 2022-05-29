#include "dns.h"
#include "user.h"
int main(int argc, char **argv)
{
    if (argc != 2) {
        printf("Wrong number of parameters\n");
        exit(0);
    }

    dns(argv[1]);

    exit(0);
    return 0;
}