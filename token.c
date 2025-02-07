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
    int tokens = 0;  // To track the number of tokens in the bucket

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
        // Refill tokens based on the output rate
        tokens = (tokens + o_rate <= b_size) ? tokens + o_rate : b_size;

        if (packet_sz[i] > tokens) {
            printf("\n\nIncoming packet size (%d bytes) exceeds available tokens (%d) - PACKET REJECTED!", packet_sz[i], tokens);
        } else {
            tokens -= packet_sz[i];
            printf("\n\nIncoming Packet size: %d bytes", packet_sz[i]);
            printf("\nTokens remaining to transmit: %d", tokens);

            // Generate random transmission time
            p_time = randm(4);
            printf("\nEstimated Time for transmission: %d units", p_time);

            for (clk = 10; clk <= p_time; clk += 10) {
                sleep(1); // Simulate time delay

                if (packet_sz[i] > 0) {
                    op = (packet_sz[i] <= o_rate) ? packet_sz[i] : o_rate;
                    packet_sz[i] -= op;
                    printf("\nTransmitted Packet of size %d bytes ----> Remaining Bytes: %d", op, packet_sz[i]);
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
