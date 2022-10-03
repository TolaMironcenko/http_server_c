#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <unistd.h> // системная библиотека unix
#include <string.h>
#include "defines.h"
#include "http_logic.c"

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

    int result = bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));
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

            char request_method[10];
            char link[50];
            getrequestmethod(request_method, buffer);
            getlink(link, buffer);
            printf("link = %s\n", link);
            printf("request_method = %s\n", request_method);

            if (strcmp(request_method, "GET") == 0)
            {
                if (strcmp(link, "/") == 0 || strcmp(link, "/home/") == 0 || strcmp(link, "/home") == 0)
                {
                    response = "HTTP/1.1 200 OK\r\nVersion: HTTP/1.1\r\nContent-Type: text/html; charset=utf-8\r\nContent-Length: 1024\r\n\r\n<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\"><meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"><title>index</title></head><body><h1>index</h1></body></html>";
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
                else
                {
                    response = "HTTP/1.1 404 NOT FOUND\r\nVersion: HTTP/1.1\r\nContent-Type: text/html; charset=utf-8\r\nContent-Length: 20\r\n\r\n<h1>NOT FOUND</h1>";
                    result = send(client_socket, response, strlen(response), 0);
                    if (result < 0)
                    {
                        perror("[-] send error");
                    }
                    else if (result > 0)
                    {
                        printf("HTTP/1.1 404 NOT FOUND\n");
                    }
                    // close(client_socket);
                }
            }
        }
    }

    close(server_socket);

    return 0;
}