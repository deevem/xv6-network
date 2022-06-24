#include "../kernel/types.h"
#include "../kernel/stat.h"
#include "../kernel/param.h"
#include "user.h"

#define BUFFER_SIZE 20480
#define LINE_SIZE 1024

char buffer[BUFFER_SIZE + 10];
int content_len = -1;
int chunked = 0;


int parse_content_len(char *line) {
    if (strlen(line) < 17)
        return -1;
    
    char *str = "Content-Length: ";
    int i = 0;
    while (line[i] == str[i])
        i++;
    if (!str[i]) {
        int len = 0;
        while (line[i]) {
            len = len * 10 + (line[i++] - '0');
        }
        if (len >= 0)
            content_len = len;
        return len;
    }
    return -1;
}

int is_chunked(char *line) {
    int res = strcmp(line, "Transfer-Encoding: chunked");
    if (res == 0)
        chunked = 1;
    return res;
}

char * parse_http_header(int fd, int *rn) {
    int n = read(fd, buffer, BUFFER_SIZE);
    int lb = 0;
    if (n < 0)
        return 0;
    for (int i = 0; i < n; i++) {
        if (buffer[i] == '\r') {
            char line[LINE_SIZE];
            memset(line, 0, 300);
            int line_size = i - lb;
            memmove(line, buffer + lb, line_size);

            parse_content_len(line);
            is_chunked(line);

            i++; // remove \n
            lb = i + 1; // remove \r \n
            if (line_size == 0)
                break;
        }
    }

    *rn = n - lb;
    return buffer + lb;
}


int main(int argc, char *argv[]) {
    if (argc != 2){
        printf("usage wget url\n");
        exit(0);
    }

    uint32_t ip = 0;
    uint32_t port = 80;

    char input[100] = {0};
    memmove(input, argv[1], strlen(argv[1]));
    int i = 0;

    char *path = "/";
    char host[100] = {0};
    while (input[i] && input[i] != '/')
        i++;
    if (input[i] == '/')
        path = input + i;
    
    memmove(host, input, i);
    argv[1] = host;
    
    ip = dns(argv[1]);

    int fd = connect_tcp(ip, port);

    char msg[200] = {0};
    snprintf(msg, 200, "GET %s HTTP/1.1\r\nHost: %s\r\n\r\n", path, host);

    write(fd, msg, strlen(msg));

    int rn = 0;
    char *d = parse_http_header(fd, &rn);
    
    printf("%d %d", rn, content_len);

    if (content_len > 0) {
        printf("content len\n");
        int n;
        write(1, d, content_len < rn ? content_len : rn);
        content_len -= rn;
        if (content_len > 0) {
            while ((n = read(fd, buffer, BUFFER_SIZE)) > 0) {
                printf("%d\n", n);
                //write(1, buffer, content_len < n ? content_len : n);
                content_len -= n;
                if (content_len == 0)
                    break;
            }
        }
    } else if (chunked) {
        printf("test chunk\n");
    } else {
        write(1, d, rn);
        int n;
        while ((n = read(fd, buffer, BUFFER_SIZE)) > 0) {
            write(1, buffer, n);
        }
    }

    exit(0);
}