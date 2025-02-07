#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MAXLINE 1024

int main(int argc, char *argv[]) {
    int sock_fd, n;
    struct sockaddr_in servaddr;
    char buf[MAXLINE];
    char address_buf[MAXLINE], message_buf[MAXLINE];
    
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <server_address> <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    
    if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(atoi(argv[2]));
    inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
    
    if (connect(sock_fd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }
    
    recv(sock_fd, buf, MAXLINE, 0);
    printf("S: %s", buf);
    
    snprintf(buf, sizeof(buf), "HELO client_mail_server\r\n");
    send(sock_fd, buf, strlen(buf), 0);
    recv(sock_fd, buf, MAXLINE, 0);
    printf("S: %s", buf);
    
    printf("Enter sender email: ");
    fgets(address_buf, sizeof(address_buf), stdin);
    address_buf[strcspn(address_buf, "\n")] = 0;
    snprintf(buf, sizeof(buf), "MAIL FROM: <%s>\r\n", address_buf);
    send(sock_fd, buf, strlen(buf), 0);
    recv(sock_fd, buf, MAXLINE, 0);
    printf("S: %s", buf);
    
    printf("Enter recipient email: ");
    fgets(address_buf, sizeof(address_buf), stdin);
    address_buf[strcspn(address_buf, "\n")] = 0;
    snprintf(buf, sizeof(buf), "RCPT TO: <%s>\r\n", address_buf);
    send(sock_fd, buf, strlen(buf), 0);
    recv(sock_fd, buf, MAXLINE, 0);
    printf("S: %s", buf);
    
    snprintf(buf, sizeof(buf), "DATA\r\n");
    send(sock_fd, buf, strlen(buf), 0);
    recv(sock_fd, buf, MAXLINE, 0);
    printf("S: %s", buf);
    
    printf("Enter email content. End with a single period (.) on a line:\n");
    while (1) {
        fgets(message_buf, sizeof(message_buf), stdin);
        if (strcmp(message_buf, ".\n") == 0) break;
        send(sock_fd, message_buf, strlen(message_buf), 0);
    }
    send(sock_fd, ".\r\n", 3, 0);
    recv(sock_fd, buf, MAXLINE, 0);
    printf("S: %s", buf);
    
    snprintf(buf, sizeof(buf), "QUIT\r\n");
    send(sock_fd, buf, strlen(buf), 0);
    recv(sock_fd, buf, MAXLINE, 0);
    printf("S: %s", buf);
    
    close(sock_fd);
    return 0;
}