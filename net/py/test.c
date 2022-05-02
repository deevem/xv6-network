#include "string.h"
#include <stdio.h>

static void
encode_qname(char *qn, char *host)
{
  char *l = host; 
  
  for(char *c = host; c < host+strlen(host)+1; c++) {
    if(*c == '.') {
      *qn++ = (char) (c-l);
      for(char *d = l; d < c; d++) {
        *qn++ = *d;
      }
      l = c+1; // skip .
    }
  }
  *qn = '\0';
}


static void myencoder(char* qname, char* host) {
    int pos = 0;
    for (int i = 0; i < strlen(host); i++) {
        if (host[i] != '.')
            qname[i + 1] = host[i];
        else {
            qname[pos] = i - pos;
            pos = i + 1;
        }
    }
}

// Decode a DNS name
static void
decode_qname(char *qn)
{
  while(*qn != '\0') {
    int l = *qn;
    if(l == 0)
      break;
    for(int i = 0; i < l; i++) {
      *qn = *(qn+1);
      qn++;
    }
    *qn++ = '.';
  }
}

int main() {
    char a[1000];
    char c[1000];
    char *b = "pdos.csail.mit.edu.";
    memset(a, 0, 1000);
    memset(c, 0, 1000);
    encode_qname(a, b);
    myencoder(c, b);
    for (int i = 0; i < strlen(a) +3; i++) {
        printf("%d %c %d %c %d\n", i, a[i], (char)(a[i]), c[i], (char)(c[i]));
    }
}