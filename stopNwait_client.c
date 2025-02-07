#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <time.h>
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
    struct sockaddr_in serverAddr;
    socklen_t addr_size;

    int frame_id = 0;
    Frame frame_send;
    Frame frame_recv;
    int ack_recv = 1;
    int resend = 0;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    memset(&serverAddr, '\0', sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    srand(time(NULL));

    struct timeval timeout;
    timeout.tv_sec = TIMEOUT_SECS;
    timeout.tv_usec = 0;
    
    if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout)) < 0) {
        perror("setsockopt failed");
        exit(1);
    }

    for (int i = 0; i < 20; i++) {
        if (ack_recv == 1 || resend == 1) {
            frame_send.sq_no = frame_id;
            frame_send.frame_kind = 1;
            frame_send.ack = 0;

            for (int j = 0; j < 10; j++) {
                frame_send.packet.data[j] = 'A' + (rand() % 26);
            }
            frame_send.packet.data[10] = '\0';

            sendto(sockfd, &frame_send, sizeof(Frame), 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
            printf("[+]Frame Send with sq_no %d, Data: %s\n", frame_send.sq_no, frame_send.packet.data);
            ack_recv = 0;
            resend = 0;
        }

        addr_size = sizeof(serverAddr);

        int f_recv_size = recvfrom(sockfd, &frame_recv, sizeof(frame_recv), 0, (struct sockaddr*)&serverAddr, &addr_size);

        if (f_recv_size == -1) {
            if (errno == EWOULDBLOCK) {
                printf("[-]Timeout occurred for sq_no %d. Resending Frame\n", frame_send.sq_no);
                resend = 1;
                continue;
            } else {
                perror("recvfrom error");
                exit(1);
            }
        }

        if (f_recv_size > 0) {
            if (frame_recv.sq_no == 0 && frame_recv.ack == frame_id + 1) {
                printf("[+]Ack Received for sq_no %d\n", frame_id);
                ack_recv = 1;
                frame_id++;
            } else {
                printf("[-]Incorrect Ack Received. Expected ack for sq_no %d, got ack for %d\n", frame_id, frame_recv.ack - 1);
            }
        }
    }

    close(sockfd);
    return 0;
}