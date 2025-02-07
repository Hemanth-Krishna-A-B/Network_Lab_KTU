#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT_ADDR 8080
#define MSG_SIZE 1024

int main() 
{
    int my_server;
    struct sockaddr_in server_addr, client_addr;
    char buffer[MSG_SIZE] = {0};
    socklen_t addrlen = sizeof(client_addr);

    my_server = socket(AF_INET, SOCK_DGRAM, 0);
    if (my_server < 0) 
        return EXIT_FAILURE;
  
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT_ADDR);

    if (bind(my_server, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) 
    {

        close(my_server);
        return EXIT_FAILURE;
    }

    printf("UDP server is listening on port %d\n", PORT_ADDR);

    int valread = recvfrom(my_server, buffer, MSG_SIZE, 0, (struct sockaddr *)&client_addr, &addrlen);
    if (valread > 0) 
    {
        buffer[valread] = '\0'; 
        printf("Message from client: %s\n", buffer);

      
        sendto(my_server, "Hello from server", strlen("Hello from server"), 0, (struct sockaddr *)&client_addr, addrlen);
        printf("Hello message sent to client\n");
    }

    close(my_server);
    return 0;
}


