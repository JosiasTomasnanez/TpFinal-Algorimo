
class Dijkstra {
private:
  int n;   // Número de nodos
  int **C; // Matriz dinámica para costos
  int *P;  // Predecesores
  int s;
  int t;
  int proximoNodo(int *predecesor, int s, int t);
  int encontrarMinimo(const int *distancia, const bool *visitado, int n);

public:
  void set_C(int **c) { C = c; };
  Dijkstra(int n, int **C) {
    this->n = n;
    this->C = C;
    P = new int[n];
  }
  int dijkstra(int s, int t);
  void imprimirCamino(int *predecesor, int s, int t);
};