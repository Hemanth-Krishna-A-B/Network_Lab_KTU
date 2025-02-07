#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_NODES 4

typedef struct {
    int sender;
    int sequence_number;
    int neighbors[MAX_NODES]; 
} LSP;

void dijkstra(int graph[MAX_NODES][MAX_NODES], int src, int dist[], int prev[]) {
    int sptSet[MAX_NODES] = {0};
    for (int i = 0; i < MAX_NODES; i++) {
        dist[i] = INT_MAX;
        prev[i] = -1;
    }
    dist[src] = 0;

    for (int count = 0; count < MAX_NODES - 1; count++) {
        int u = -1;
        for (int v = 0; v < MAX_NODES; v++) {
            if (!sptSet[v] && (u == -1 || dist[v] < dist[u]))
                u = v;
        }
        if (u == -1) break;
        sptSet[u] = 1;
        for (int v = 0; v < MAX_NODES; v++) {
            if (!sptSet[v] && graph[u][v] && dist[u] != INT_MAX && dist[u] + graph[u][v] < dist[v]) {
                dist[v] = dist[u] + graph[u][v];
                prev[v] = u;
            }
        }
    }
}

int main() {
    int network[MAX_NODES][MAX_NODES] = {
        {0, 1, INT_MAX, 3},
        {1, 0, 1, INT_MAX},
        {INT_MAX, 1, 0, 2},
        {3, INT_MAX, 2, 0}
    };

    LSP lsp[MAX_NODES];

    for (int i = 0; i < MAX_NODES; i++) {
        lsp[i].sender = i;
        lsp[i].sequence_number = 0;
        for (int j = 0; j < MAX_NODES; j++) {
            lsp[i].neighbors[j] = network[i][j];
        }
    }

    LSP topology_database[MAX_NODES]; 

    for (int i = 0; i < MAX_NODES; i++) {
        topology_database[i] = lsp[i]; 
    }


    for (int i = 0; i < MAX_NODES; i++) {
        int graph[MAX_NODES][MAX_NODES] = {0}; 

       
        for (int j = 0; j < MAX_NODES; j++) { 
            for (int k = 0; k < MAX_NODES; k++) { 
                if (topology_database[j].neighbors[k] != INT_MAX) {
                    graph[j][k] = topology_database[j].neighbors[k];
                }
            }
        }

        int dist[MAX_NODES];
        int prev[MAX_NODES];
        dijkstra(graph, i, dist, prev);

        printf("Shortest paths from Node %d:\n", i);
        for (int j = 0; j < MAX_NODES; j++) {
            printf("To %d: Distance = %d\n", j, dist[j]);
        }
        printf("\n");
    }

    return 0;
}