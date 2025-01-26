#include <bitset>
using namespace std;

template <typename T>
class Cola;
class Router;
class Terminal;

struct coneccionTerminal{
Terminal* terminal;
int anchoDeBanda;
};

struct Paquete{
bitset<16> destino;
bitset<16> origen;
int tamano;
int numeroPaquete;
int id;
int totalPaquete;
string datopaquete;
};
struct coneccionVecino{
  int anchoDeBanda;
  Router* vecino;
  bitset<16> ipVecino;
  Cola<Paquete>* colaEnvio;
};
struct Pagina{
bitset<16> destino;
bitset<16> origen;
int tamano;
int id;
string datopagina;
};
