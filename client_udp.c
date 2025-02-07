
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() 
{
    int sock;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};


    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) 
        return EXIT_FAILURE;
    

   
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
    {
        close(sock);
        return EXIT_FAILURE;
    }

    sendto(sock, "Hello from client", strlen("Hello from client"), 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    printf("Hello message sent\n");

    socklen_t addrlen = sizeof(serv_addr);
    int valread = recvfrom(sock, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&serv_addr, &addrlen);
    if (valread > 0)
    {
        buffer[valread] = '\0'; 
        printf("Message from server: %s\n", buffer);
    }

    close(sock);
    return 0;
}

