#ifndef HTTP_SERVER_C_HTTP_LOGIC_C
#define HTTP_SERVER_C_HTTP_LOGIC_C

#include <stdio.h>
#include <string.h>

void getrequestmethod(char *request_method, char buffer[MAX_BUFFER_SIZE])
{
    int pos = 0;

    memset(request_method, '\0', 10);
    while (buffer[pos] != ' ')
    {
        request_method[pos] = buffer[pos];
        pos++;
    }
    request_method[pos] = '\0';
}

void getlink(char *link, char buffer[MAX_BUFFER_SIZE])
{
    int pos = 0;
    int i = 0;

    memset(link, '\0', 50);
    while (buffer[pos] != '/')
    {
        pos++;
    }

    while (buffer[pos] != ' ')
    {
        link[i] = buffer[pos];
        pos++;
        i++;
    }
}

#endif //HTTP_SERVER_C_HTTP_LOGIC_C