#include "../kernel/types.h"
#include "../kernel/stat.h"
#include "../kernel/param.h"
#include "user.h"

#define MAKE_IP_ADDR(a, b, c, d) (((uint32_t)a << 24) | ((uint32_t)b << 16) | ((uint32_t)c << 8) | (uint32_t)d)

char* strcat(char* str1, char* str2)
{
  char * result = (char *) malloc(strlen(str1) +  strlen(str2) + 1);
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
        printf("Usage: new_group group_name \n");
        exit(1);
    }

    if (strlen(argv[1]) > 20)
    {
        printf("the name should not longer than 20 chars");
        exit(1);
    }

    char * output = strcat("new_group", argv[1]);
    int fd = connect(MAKE_IP_ADDR(183,172,152,232), 12345, 54321); // the ip of serve is hard code, may change it future
    if (write(fd, output, strlen(output)) < 0) {
        printf("udp tx failed\n");
        free(output);
        exit(1);
    } else {
        printf("packet sent\n");
    }
    free(output);
    exit(0);

}