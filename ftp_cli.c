#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERV_TCP_PORT 5035
#define MAX 4096

int main(int argc, char *argv[]) {
    int sockfd, n;
    struct sockaddr_in serv_addr;
    char sendline[MAX], recvline[MAX];
    FILE *file;

    // Create socket
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Initialize server address structure
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERV_TCP_PORT);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Connect to server
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    printf("Connected to FTP server.\n");

    // Get filename from user
    printf("Enter the source file name: ");
    fgets(sendline, MAX, stdin);
    sendline[strcspn(sendline, "\n")] = 0; // Remove newline

    // Send filename to server
    if (write(sockfd, sendline, strlen(sendline)) < 0) {
        perror("Error sending filename");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Open file to save received data
    file = fopen("received_file", "wb");
    if (file == NULL) {
        perror("Failed to create file");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Read and write file content from server
    while ((n = read(sockfd, recvline, MAX)) > 0) {
        fwrite(recvline, 1, n, file);
    }

    if (n < 0) {
        perror("Error reading from server");
    } else {
        printf("File received successfully. Saved as 'received_file'.\n");
    }

    // Close file and socket
    fclose(file);
    close(sockfd);
    return 0;
}
