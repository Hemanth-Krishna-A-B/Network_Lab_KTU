#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define NOF_PACKETS 10  // Number of packets

// Function to generate random packet sizes (10, 20, 30, ..., 60)
int randm(int a) {
    int rn = (rand() % a) + 1;
    return rn * 10;  // Ensure packet size is a multiple of 10
}

int main() {
    int packet_sz[NOF_PACKETS], i, clk, b_size, o_rate, p_sz_rm = 0, p_time, op;

    // Seed random number generator
    srand(time(NULL));

    // Generate random packet sizes
    for (i = 0; i < NOF_PACKETS; ++i)
        packet_sz[i] = randm(6);

    // Display generated packet sizes
    printf("\nGenerated Packets:\n");
    for (i = 0; i < NOF_PACKETS; ++i)
        printf("Packet[%d]: %d bytes\n", i + 1, packet_sz[i]);

    // Get Output Rate and Bucket Size from user
    printf("\nEnter the Output Rate: ");
    scanf("%d", &o_rate);
    printf("Enter the Bucket Size: ");
    scanf("%d", &b_size);

    // Validate inputs
    if (o_rate <= 0 || b_size <= 0) {
        printf("Invalid input! Output rate and bucket size must be positive.\n");
        return EXIT_FAILURE;
    }

    // Process each packet
    for (i = 0; i < NOF_PACKETS; ++i) {
        if ((packet_sz[i] + p_sz_rm) > b_size) {
            if (packet_sz[i] > b_size) {
                printf("\n\nIncoming packet size (%d bytes) exceeds bucket capacity (%d bytes) - PACKET REJECTED!", packet_sz[i], b_size);
            } else {
                printf("\n\nBucket capacity exceeded - PACKET REJECTED!");
            }
        } else {
            p_sz_rm += packet_sz[i];
            printf("\n\nIncoming Packet size: %d bytes", packet_sz[i]);
            printf("\nBytes remaining to Transmit: %d", p_sz_rm);
            
            // Generate random transmission time
            p_time = randm(4);
            printf("\nEstimated Time for transmission: %d units", p_time);

            for (clk = 10; clk <= p_time; clk += 10) {
                sleep(1); // Simulate time delay

                if (p_sz_rm > 0) {
                    op = (p_sz_rm <= o_rate) ? p_sz_rm : o_rate;
                    p_sz_rm -= op;
                    printf("\nTransmitted Packet of size %d bytes ----> Remaining Bytes: %d", op, p_sz_rm);
                } else {
                    printf("\nTime left for transmission: %d units", p_time - clk);
                    printf("\nNo packets to transmit!");
                }
            }
        }
    }
    printf("\n\nAll packets processed successfully!\n");
    return 0;
}
