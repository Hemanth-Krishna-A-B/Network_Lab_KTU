#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_NODES 4

typedef struct {
    int distance;
    int next_hop;
} RoutingTableEntry;

void printRoutingTable(RoutingTableEntry table[], int node) {
    printf("Routing Table for Node %d:\n", node);
    printf("Dest\tDist\tNext Hop\n");
    for (int i = 0; i < MAX_NODES; i++) {
        printf("%d\t%d\t%d\n", i, table[i].distance, table[i].next_hop);
    }
    printf("\n");
}

int main() {
    int costMatrix[MAX_NODES][MAX_NODES] = {
        {0, 1, INT_MAX, 3},
        {1, 0, 1, INT_MAX},
        {INT_MAX, 1, 0, 2},
        {3, INT_MAX, 2, 0}
    };

    RoutingTableEntry routingTables[MAX_NODES][MAX_NODES];


    for (int i = 0; i < MAX_NODES; i++) {
        for (int j = 0; j < MAX_NODES; j++) {
            routingTables[i][j].distance = costMatrix[i][j];
            routingTables[i][j].next_hop = (costMatrix[i][j] == INT_MAX) ? -1 : j;
        }
    }

   
    for (int k = 0; k < MAX_NODES - 1; k++) { 
        for (int i = 0; i < MAX_NODES; i++) { 
            for (int j = 0; j < MAX_NODES; j++) { 
                for (int n = 0; n < MAX_NODES; n++) { 
                    if (costMatrix[i][n] != INT_MAX && routingTables[j][n].distance != INT_MAX) {
                        int newDist = costMatrix[i][n] + routingTables[j][n].distance;
                        if (newDist < routingTables[j][i].distance) {
                            routingTables[j][i].distance = newDist;
                            routingTables[j][i].next_hop = n;
                        }
                    }
                }
            }
        }
    }

    for (int i = 0; i < MAX_NODES; i++) {
        printRoutingTable(routingTables[i], i);
    }

    return 0;
}