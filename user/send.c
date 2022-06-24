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


int main(int argc, char**argv){
    if (argc <= 2)
    {
        printf("Usage:  send group_name message \n");
        exit(1);
    }

    if (strlen(argv[1]) > 20)
    {
        printf("the name should not longer than 20 chars");
        exit(1);
    }

    int n = argc - 2;
    int num = 0;
    for (int i = 0; i < n; i++)
    {
      num += strlen(argv[i+2]);
    }

    if (num > 100)
    {
        printf("the message should not larger than 100 chars");
        exit(1);
    }
    char * first = argv[0];
    char * output = first;
    for (int i = 0; i < argc - 1; i++)
    {
      output = strcat(first, argv[i + 1]);
      if (i > 0)
        {
          free(first);
        }
      first = output;
      
    }
    int fd = connect(MAKE_IP_ADDR(SERVER_IP1, SERVER_IP2, SERVER_IP3, SERVER_IP4), 12345, 54321); // the ip of serve is hard code, may change it future
    if (write(fd, output, strlen(output)) < 0) {
        printf("udp tx failed\n");
        free(output);
        close(fd);
        exit(1);
    }
    free(output);
    close(fd);
    exit(0);
}