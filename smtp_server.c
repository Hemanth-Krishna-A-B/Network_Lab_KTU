#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MAXLINE 1024
#define PORT 2525

void handle_client(int client_fd);

int main() {
    int server_fd, client_fd;
    struct sockaddr_in servaddr, cliaddr;
    socklen_t cli_len = sizeof(cliaddr);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("Bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 5) < 0) {
        perror("Listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("SMTP Server running on port %d...\n", PORT);

    while (1) {
        client_fd = accept(server_fd, (struct sockaddr *)&cliaddr, &cli_len);
        if (client_fd < 0) {
            perror("Accept failed");
            continue;
        }
        printf("Connection accepted from %s:%d\n", inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port));
        handle_client(client_fd);
    }

    close(server_fd);
    return 0;
}

void handle_client(int client_fd) {
    char buffer[MAXLINE];
    ssize_t n;

    // Send 220 greeting
    snprintf(buffer, sizeof(buffer), "220 Simple SMTP Server\r\n");
    send(client_fd, buffer, strlen(buffer), 0);

    // Receive HELO
    n = recv(client_fd, buffer, MAXLINE, 0);
    if (n <= 0) { close(client_fd); return; }
    buffer[n] = '\0';
    printf("C: %s", buffer);

    snprintf(buffer, sizeof(buffer), "250 Hello, pleased to meet you\r\n");
    send(client_fd, buffer, strlen(buffer), 0);

    // Receive MAIL FROM
    n = recv(client_fd, buffer, MAXLINE, 0);
    if (n <= 0) { close(client_fd); return; }
    buffer[n] = '\0';
    printf("C: %s", buffer);

    snprintf(buffer, sizeof(buffer), "250 Sender OK\r\n");
    send(client_fd, buffer, strlen(buffer), 0);

    // Receive RCPT TO
    n = recv(client_fd, buffer, MAXLINE, 0);
    if (n <= 0) { close(client_fd); return; }
    buffer[n] = '\0';
    printf("C: %s", buffer);

    snprintf(buffer, sizeof(buffer), "250 Recipient OK\r\n");
    send(client_fd, buffer, strlen(buffer), 0);

    // Receive DATA
    n = recv(client_fd, buffer, MAXLINE, 0);
    if (n <= 0) { close(client_fd); return; }
    buffer[n] = '\0';
    printf("C: %s", buffer);

    snprintf(buffer, sizeof(buffer), "354 Enter mail, end with '.' on a line by itself\r\n");
    send(client_fd, buffer, strlen(buffer), 0);

    // Receive Email Content
    FILE *fp = fopen("email.txt", "w");
    if (!fp) { perror("File open failed"); close(client_fd); return; }

    while ((n = recv(client_fd, buffer, MAXLINE, 0)) > 0) {
        buffer[n] = '\0';
        printf("C: %s", buffer);
        if (strcmp(buffer, ".\r\n") == 0) break;
        fprintf(fp, "%s", buffer);
    }
    fclose(fp);

    snprintf(buffer, sizeof(buffer), "250 Message accepted for delivery\r\n");
    send(client_fd, buffer, strlen(buffer), 0);

    // Receive QUIT
    n = recv(client_fd, buffer, MAXLINE, 0);
    if (n <= 0) { close(client_fd); return; }
    buffer[n] = '\0';
    printf("C: %s", buffer);

    snprintf(buffer, sizeof(buffer), "221 Server closing connection\r\n");
    send(client_fd, buffer, strlen(buffer), 0);

    close(client_fd);
    printf("Connection closed.\n");
}