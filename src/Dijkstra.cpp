#include <cstdlib>
#include <climits>
#include "Dijkstra.h"
#include <iostream>

#define INFI INT_MAX
#define MIEMBRO 1
#define NO_MIEMBRO 0
#define MAX_SIZE_CAMINO 256

using namespace std;

int Dijkstra :: encontrarMinimo(const int* distancia, const bool* visitado, int n) {
    int minimo = INT_MAX, indice = -1;
    for (int i = 0; i < n; i++) {
        if (!visitado[i] && distancia[i] <= minimo) {
            minimo = distancia[i];
            indice = i;
        }
    }
    return indice;
}

// Implementación del algoritmo de Dijkstra
int Dijkstra ::  dijkstra(int s, int t) {
    int* distancia = new int[n];  // Array de distancias desde el nodo origen
    int* predecesor = new int[n]; // Array de predecesores
    bool* visitado = new bool[n]; // Array para marcar nodos visitados

    // Inicializar distancias, predecesores y visitados
    for (int i = 0; i < n; i++) {
        distancia[i] = INT_MAX;
        predecesor[i] = -1;
        visitado[i] = false;
    }
    distancia[s] = 0; // La distancia al nodo origen es 0

    for (int i = 0; i < n - 1; i++) {
        int u = encontrarMinimo(distancia, visitado, n);
        if (u == -1) break; // Si no hay más nodos alcanzables
        visitado[u] = true;

        for (int v = 0; v < n; v++) {
            // Actualizar la distancia al nodo vecino si se cumple la condición
            if (!visitado[v] && C[u][v] != 0 && distancia[u] != INT_MAX &&
                distancia[u] + C[u][v] < distancia[v]) {
                distancia[v] = distancia[u] + C[u][v];
                predecesor[v] = u;
            }
        }
    }

    delete[] distancia;  // Liberar memoria para el array de distancias
    delete[] visitado;   // Liberar memoria para el array de visitados
    return proximoNodo(predecesor, s, t); // Devolver el array de predecesores
}

int Dijkstra :: proximoNodo(int* predecesor, int s, int t) {
    
    if (predecesor[t] == -1) {
        
        return -1; // Indicador de que no hay camino
    }
    

    if (s == t) {
        return s; // Si origen y destino son el mismo nodo
    }

    // Retroceder en la lista de predecesores para encontrar el primer nodo del camino
    int actual = t;
    while (predecesor[actual] != s) {
        actual = predecesor[actual];
    }
    return actual; // El primer nodo al que debemos ir
}


// Función para imprimir el camino más corto del nodo s al t
void Dijkstra :: imprimirCamino(int* predecesor, int s, int t) {
    
    
    if (predecesor[t] == -1) {
        
        return; // Indicador de que no hay camino
    }

    if (t == -1 || t == s) {
        cout << s << " ";
        return;
    }
    imprimirCamino(predecesor, s, predecesor[t]);
    cout << t << " ";
}
