#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define SERV_TCP_PORT 5035
#define MAX 4096

int main() {
    int sockfd, newsockfd;
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t clength;
    char filename[MAX];
    char buffer[MAX];
    FILE *file;

    // Create socket
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Initialize server address structure
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(SERV_TCP_PORT);

    // Bind socket
    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Bind failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Listen for connections
    if (listen(sockfd, 5) < 0) {
        perror("Listen failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    printf("Server listening on port %d...\n", SERV_TCP_PORT);

    // Accept client connection
    clength = sizeof(cli_addr);
    if ((newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clength)) < 0) {
        perror("Accept failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    printf("Client connected.\n");

    // Receive filename from client
    memset(filename, 0, MAX);
    if (read(newsockfd, filename, MAX) <= 0) {
        perror("Failed to receive filename");
        close(newsockfd);
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    filename[strcspn(filename, "\n")] = 0; 
    printf("Client requested file: %s\n", filename);

    // Open the requested file
    file = fopen(filename, "rb");
    if (file == NULL) {
        perror("File not found");
        char *error_msg = "ERROR: File not found\n";
        write(newsockfd, error_msg, strlen(error_msg));
    } else {
        // Send file contents
        size_t bytes_read;
        while ((bytes_read = fread(buffer, 1, sizeof(buffer), file)) > 0) {
            write(newsockfd, buffer, bytes_read);
        }
        printf("File transfer complete.\n");
        fclose(file);
    }

    // Close sockets
    close(newsockfd);
    close(sockfd);
    return 0;
}
