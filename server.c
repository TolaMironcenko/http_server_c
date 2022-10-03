#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <unistd.h>    // системная библиотека unix
#include <string.h>
#include "defines.h"

void getrequesttype(char *link, char buffer[MAX_BUFFER_SIZE]);

int main()
{
    int server_socket;
    struct sockaddr_in server_addr;

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0)
    {
        perror("[-] socket error");
        exit(1);
    }

    memset(&server_addr, '\0', sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(DEFAULT_PORT);
    server_addr.sin_addr.s_addr = inet_addr(DEFAULT_IP);

    int result = bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if (result < 0)
    {
        perror("[-] bind error");
        exit(1);
    }

    if (listen(server_socket, SOMAXCONN) < 0)
    {
        perror("[-] listen error");
        exit(1);
    }

    char buffer[MAX_BUFFER_SIZE];

    while (1)
    {
        int client_socket = accept(server_socket, NULL, NULL);
        if (client_socket < 0)
        {
            perror("[-] accept error");
            exit(1);
        }

        result = recv(client_socket, buffer, MAX_BUFFER_SIZE, 0);

        char *response;
        
        if (result == 0)
        {
            perror("[-] connection closed...\n");
        }
        else if (result < 0)
        {
            perror("[-] client recv error");
            close(client_socket);
        }
        else if (result > 0)
        {
            buffer[result] = '\0';
            printf("%s\n", buffer);

            char request_type[10];
            getrequesttype(request_type, buffer);
            printf("request_type = %s\n", request_type);
            int b = strcmp(request_type, "GET\0");
            printf("request_type == GET = %d\n", b);
            
            response = "HTTP/1.1 200 OK\r\nVersion: HTTP/1.1\r\nContent-Type: text/html; charset=utf-8\r\nContent-Length: 15\r\n\r\n<h1>index</h1>";

            result = send(client_socket, response, strlen(response), 0);
            if (result < 0)
            {
                perror("[-] send error");
            }
            else if (result > 0)
            {
                printf("HTTP/1.1 200 OK\n");
            }
            close(client_socket);
        }
    }

    close(server_socket);

    return 0;
}

void getrequesttype(char *request_type, char buffer[MAX_BUFFER_SIZE]) {
    int pos = 0;

    memset(request_type, '\0', 10);
    while (buffer[pos] != ' ')
    {
        request_type[pos] = buffer[pos];
        pos++;
    }
    request_type[pos] = '\0';
}