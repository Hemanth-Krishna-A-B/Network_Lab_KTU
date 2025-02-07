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
    int my_server, my_socket;
    struct sockaddr_in address;
    char buffer[MSG_SIZE] = {0};
    int addrlen = sizeof(address);

    
    my_server = socket(AF_INET, SOCK_STREAM, 0);
    if (my_server == 0) 
        return EXIT_FAILURE;
    

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT_ADDR);

    
    if (bind(my_server, (struct sockaddr *)&address, sizeof(address)) < 0) 
    {
        close(my_server);
        return EXIT_FAILURE;
    }

    if (listen(my_server, 3) < 0) 
    {
        close(my_server);
        return EXIT_FAILURE;
    }

    printf("Server is listening on port %d\n", PORT_ADDR);

    
    my_socket = accept(my_server, (struct sockaddr *)&address, (socklen_t*)&addrlen);
    if (my_socket < 0) 
    {
        close(my_server);
        return EXIT_FAILURE;
    }

    
    int valread = read(my_socket, buffer, MSG_SIZE);
    if (valread > 0) 
    {
        printf("Message from client: %s\n", buffer);
        send(my_socket, "Hello from server", strlen("Hello from server"), 0);
        printf("Hello message sent to client\n");
    }

    close(my_socket);
    close(my_server);

    return 0;
}
