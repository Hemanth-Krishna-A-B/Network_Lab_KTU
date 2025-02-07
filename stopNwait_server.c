#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <errno.h>

#define PORT 8080
#define TIMEOUT_SECS 2

typedef struct packet {
    char data[1024];
} Packet;

typedef struct frame {
    int frame_kind;
    int sq_no;
    int ack;
    Packet packet;
} Frame;

int main() {
    int sockfd;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addr_size;
    int expected_sq_no = 0;
    Frame frame_recv;
    Frame frame_send;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    memset(&serverAddr, '\0', sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("bind failed");
        exit(1);
    }

    addr_size = sizeof(clientAddr);

    struct timeval timeout;
    timeout.tv_sec = TIMEOUT_SECS;
    timeout.tv_usec = 0;

    if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout)) < 0) {
        perror("setsockopt failed");
        exit(1);
    }

    for (int i = 0; i < 20; i++) {
        int f_recv_size = recvfrom(sockfd, &frame_recv, sizeof(Frame), 0, (struct sockaddr*)&clientAddr, &addr_size);

        if (f_recv_size == -1) {
            if (errno == EWOULDBLOCK) {
                printf("[-]Timeout waiting for frame %d\n", expected_sq_no);
                continue; // Important: Continue to the next loop iteration
            } else {
                perror("recvfrom error");
                exit(1);
            }
        }

        if (f_recv_size > 0) {
            if (frame_recv.frame_kind == 1 && frame_recv.sq_no == expected_sq_no) {
                printf("[+]Frame Received: %s\n", frame_recv.packet.data);

                frame_send.sq_no = 0;
                frame_send.frame_kind = 0;
                frame_send.ack = expected_sq_no + 1;
                sendto(sockfd, &frame_send, sizeof(frame_send), 0, (struct sockaddr*)&clientAddr, addr_size);
                printf("[+]Ack Send for sq_no %d\n", expected_sq_no);
                expected_sq_no++;
            } else {
                printf("[-]Unexpected Frame or Incorrect Sequence Number. Discarded.\n");
                // Resend last ACK in case of unexpected frame
                frame_send.sq_no = 0;
                frame_send.frame_kind = 0;
                frame_send.ack = expected_sq_no;
                sendto(sockfd, &frame_send, sizeof(frame_send), 0, (struct sockaddr*)&clientAddr, addr_size);
                printf("[+]Resending Ack for sq_no %d\n", expected_sq_no-1);

            }
        }
    }

    close(sockfd);
    return 0;
}