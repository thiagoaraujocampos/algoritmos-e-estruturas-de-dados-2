// Nome: Thiago Araujo Campos 
// Engenharia de Computaçãp
// RA: 769850

// -Wall -pedantic -O2 -Wno-unused-result

#include <limits.h>
#include <iostream>
using namespace std;

int distancia_min(int dist[], bool ok[], int tam) 
{ 
    int min = INT_MAX, min_index; 
  
    for (int v = 0; v < tam; v++) 
        if (ok[v] == false && dist[v] <= min) 
            min = dist[v], min_index = v; 
  
    return min_index; 
} 

int dijkstra(int** M, int tam) {
    int dist[tam];
    bool ok[tam];

    for (int i = 0; i < tam; i++) {
        dist[i] = INT_MAX;
        ok[i] = false;
    }

    dist[0] = 0;

    for (int count = 0; count < tam - 1; count++) { 
        int u = distancia_min(dist, ok, tam);
  
        ok[u] = true; 

        for (int v = 0; v < tam; v++) 
            if (!ok[v] && M[u][v] && dist[u] != INT_MAX && dist[u] + M[u][v] < dist[v]) 
                dist[v] = dist[u] + M[u][v]; 
    }

    return dist[tam-1];
}

int main(int argc, char *argv[])
{
    int V, E, A, B, W, i, j;

    cin >> V >> E;

    int **M = (int **) malloc(V * sizeof(int *));

    for (i = 0; i < V; i++) {
        M[i] = (int *) malloc(V * sizeof(int));
        for (j = 0; j < V; j++)
            M[i][j] = 0;
    }

    for (i = 0; i < E; i++) {
        cin >> A >> B >> W;
        if (M[A][B] == 0 || W < M[A][B])
            M[A][B] = W;
    }

    cout << dijkstra(M, V);

    return 0;
}
