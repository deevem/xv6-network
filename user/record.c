#include "../kernel/types.h"
#include "../kernel/stat.h"
#include "../kernel/param.h"
#include "user.h"

#define MAKE_IP_ADDR(a, b, c, d) (((uint32_t)a << 24) | ((uint32_t)b << 16) | ((uint32_t)c << 8) | (uint32_t)d)

char* strcat(char* str1, char* str2)
{
  char * result = (char *) malloc(strlen(str1) +  strlen(str2) + 2);
  for (int i = 0; i < strlen(str1); i++)
  {
    result[i] = str1[i];
  }
  result[strlen(str1)] = ' ';
  for (int i = 0; i < strlen(str2); i++)
  {
    result[strlen(str1) + 1 + i] = str2[i];
  }
  result[strlen(str1) + strlen(str2) + 1] = '\0';
  return result;
}

int main(int argc, char**argv)
{
    if (argc == 1)
    {
        printf("Usage: record group_name \n");
        exit(1);
    }

    if (strlen(argv[1]) > 20)
    {
        printf("the name should not longer than 20 chars");
        exit(1);
    }
    char * output = strcat("record", argv[1]);
    int fd = connect(MAKE_IP_ADDR(SERVER_IP1, SERVER_IP2, SERVER_IP3, SERVER_IP4), 12345, 54321);
    if (write(fd, output, strlen(output)) < 0) {
        printf("udp tx failed\n");
        free(output);
        exit(1);
    }
    free(output);
    close(fd);
    char ibuf[256];
    memset(&ibuf, 0, sizeof(ibuf));
    int fd2 = connect(MAKE_IP_ADDR(SERVER_IP1, SERVER_IP2, SERVER_IP3, SERVER_IP4), 2000, 54321);

    int cc = read(fd2, ibuf, sizeof(ibuf)-1);
    if (cc < 0)
    {
        printf("record: recv() failed\n");
        close(fd2);
        exit(1);
    }
    printf("%s", ibuf);
    close(fd2);
    exit(0);
}