#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_NODES 100

void dijkstra(int graph[MAX_NODES][MAX_NODES], int n, int src, int dist[], int prev[]) {
    int visited[MAX_NODES] = {0};

    for (int i = 0; i < n; i++) {
        dist[i] = INT_MAX;
        prev[i] = -1;
    }
    dist[src] = 0;

    for (int count = 0; count < n - 1; count++) {
        int minDist = INT_MAX, u = -1;
        for (int v = 0; v < n; v++) {
            if (!visited[v] && dist[v] < minDist) {
                minDist = dist[v];
                u = v;
            }
        }
        
        if (u == -1) break;  // No more reachable nodes
        visited[u] = 1;

        for (int v = 0; v < n; v++) {
            if (!visited[v] && graph[u][v] != INT_MAX && dist[u] != INT_MAX && 
                dist[u] + graph[u][v] < dist[v]) {
                dist[v] = dist[u] + graph[u][v];
                prev[v] = u;
            }
        }
    }
}

int main() {
    int n;
    printf("Enter the number of nodes: ");
    scanf("%d", &n);

    int graph[MAX_NODES][MAX_NODES];

    printf("Enter the adjacency matrix (use -1 for no connection):\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int value;
            scanf("%d", &value);
            graph[i][j] = (value == -1) ? INT_MAX : value;  // Convert -1 to INT_MAX
        }
    }

    for (int i = 0; i < n; i++) {
        int dist[MAX_NODES], prev[MAX_NODES];
        dijkstra(graph, n, i, dist, prev);

        printf("\nShortest paths from Node %d:\n", i);
        for (int j = 0; j < n; j++) {
            if (dist[j] == INT_MAX)
                printf("To %d: No Path\n", j);
            else
                printf("To %d: Distance = %d\n", j, dist[j]);
        }
    }

    return 0;
}
