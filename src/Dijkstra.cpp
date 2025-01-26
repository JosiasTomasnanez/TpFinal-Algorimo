#include <cstdlib>
#include <climits>
#include "Dijkstra.h"

#define INFI INT_MAX
#define MIEMBRO 1
#define NO_MIEMBRO 0
#define MAX_SIZE_CAMINO 256

using namespace std;

int* Dijkstra::dijkstra(int s, int t) {
    int *D = new int[n];  // Distancias
    int *S = new int[n];  // Conjunto S
    int actual, k, b;
    int menordist, nuevadist;

    // Inicialización
    for (int i = 0; i < n; i++) {
        S[i] = NO_MIEMBRO;
        D[i] = INFI;
        P[i] = -1;
    }
    S[s] = MIEMBRO;
    D[s] = 0;
    actual = s;
    b = 1;

    while (actual != t && b == 1) {
        b = 0;
        menordist = INFI;

        for (int i = 0; i < n; i++) {
            if (S[i] == NO_MIEMBRO) {
                nuevadist = D[actual] + C[actual][i];
                if (nuevadist < D[i]) {
                    D[i] = nuevadist;
                    P[i] = actual;
                    b = 1;
                }
                if (D[i] < menordist) {
                    menordist = D[i];
                    k = i;
                    b = 1;
                }
            }
        }

        if (b == 1) {
            actual = k;
            S[actual] = MIEMBRO;
        } else {
            break;  // Si no se encontró una mejora, terminamos
        }
    }

    // Si no se pudo alcanzar el nodo t, liberar la memoria y devolver nullptr
    if (D[t] == INFI) {
        delete[] D;
        delete[] S;
        return nullptr;  // Nodo no alcanzable
    }

    delete[] S;  // Liberar memoria para S

    // Llamamos a la función obtenerCamino para devolver el camino
    return obtenerCamino(s, t);
}

// Función para obtener el camino desde el nodo s hasta el nodo t
int* Dijkstra::obtenerCamino(int s, int t) {
    // El tamaño máximo del camino es igual al número de nodos
    int *camino = new int[MAX_SIZE_CAMINO]; // Asumimos que el número máximo de nodos es 256
    int index = 0; // Índice para el arreglo

    // Si no hay camino
    if (P[t] == -1) {
        delete[] camino;
        return nullptr;  // No hay camino
    }

    // Sigue los predecesores desde t hasta s
    for (int nodo = t; nodo != s; nodo = P[nodo]) {
        camino[index++] = nodo; // Guarda el nodo en el arreglo
    }
    camino[index++] = s; // Agrega el nodo de inicio al final del camino

    // El camino debe ser invertido para ir de s a t
    int* caminoInvertido = new int[index];
    for (int i = 0; i < index; i++) {
        caminoInvertido[i] = camino[index - 1 - i];  // Invertimos el orden
    }

    delete[] camino;  // Liberamos el arreglo original
    return caminoInvertido;  // Devolvemos el camino invertido
}
