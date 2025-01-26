#include <bitset>
#include <iostream>
using namespace std;
template <class T> class NodoArbolBinario {
private:

  NodoArbolBinario<T> *derecha;   ///< Puntero al nodo derecho.
  NodoArbolBinario<T> *izquierda; ///< Puntero al nodo izquierdo.
  T dato;
  
public:
  NodoArbolBinario()
      : derecha(nullptr), izquierda(nullptr) , dato(T()){};
  NodoArbolBinario<T> *&get_der(void) { return derecha; };
  void set_der(NodoArbolBinario<T> *n) { derecha = n; };
  NodoArbolBinario<T> *&get_izq() { return izquierda; };
  void set_izq(NodoArbolBinario<T> *n) { izquierda = n; };
  T get_dato(){return dato;};
  void set_dato(T d){dato=d;};
};
template <class T> class ArbolBinario1Byte {
private:
 bitset<8> actual;
 int profundidad=8;
 int contador;
  NodoArbolBinario<T> *raiz; ///< Puntero a la raíz del árbol.
  void eliminarRecursivo(NodoArbolBinario<T> *nodo) {
    if (nodo != nullptr) {
      eliminarRecursivo(nodo->get_izq());
      eliminarRecursivo(nodo->get_der());
      delete nodo;
    }
  }

public:
  ArbolBinario1Byte() { contador=0; raiz=nullptr;
  };
  ~ArbolBinario1Byte<T>() { eliminarRecursivo(raiz); };
  T buscarDato(bitset<16> posicion);
   void addArbol(T dato) ;
   int size(){return contador;};
   void addArbolrec(NodoArbolBinario<T>*& nodo,T dato);
   T get(int i);
};

template <class T>
T ArbolBinario1Byte<T>::get(int i) {
    // Convertir el índice entero a un bitset<16> (para ser compatible con buscarDato).
    bitset<16> posicion(i);
    // Usar buscarDato para obtener el valor. Si no se encuentra, devolver T().
    NodoArbolBinario<T>* nodo = raiz;
    if (!nodo) {
        return T();  // Árbol vacío, devolver valor por defecto.
    }
    return buscarDato(posicion);
}



  template <class T>
T ArbolBinario1Byte<T>::buscarDato(bitset<16> posicion) {
    bitset<8> aux(posicion.to_ulong() & 0xFF);
    NodoArbolBinario<T>* nodo = raiz;

    // Si la raíz es nula, devuelve el valor por defecto.
    if (!nodo) {
        return T();
    }

    for (int i = 0; i < profundidad; i++) {

        if (aux[i] == 1) {
            // Si no existe el hijo derecho, créalo y asígnalo al árbol.
            if (nodo->get_der() == nullptr) {
                nodo->set_der(new NodoArbolBinario<T>());
            }
            nodo = nodo->get_der();
        } else {
            // Si no existe el hijo izquierdo, créalo y asígnalo al árbol.
            if (nodo->get_izq() == nullptr) {
                nodo->set_izq(new NodoArbolBinario<T>());
            }
            nodo = nodo->get_izq();
        }
    }


    // Devuelve el dato almacenado en el nodo.
    return nodo->get_dato();
}



template <class T>
void ArbolBinario1Byte<T>::addArbol(T dato) {
    if (raiz == nullptr) {
        raiz = new NodoArbolBinario<T>();
    }
    NodoArbolBinario<T>* nodo = raiz;

    for (int i = 0; i < profundidad; i++) {
        if (actual[i] == 1) {
            if (nodo->get_der() == nullptr) {
                nodo->set_der(new NodoArbolBinario<T>());
            }
            nodo = nodo->get_der();
        } else {
            if (nodo->get_izq() == nullptr) {
                nodo->set_izq(new NodoArbolBinario<T>());
            }
            nodo = nodo->get_izq();
        }

        if (i == profundidad - 1) {
            nodo->set_dato(dato);
        }
    }

    contador++;
    // Incrementar el bitset actual.
    unsigned long numero = actual.to_ulong();
    numero += 1;
    actual = bitset<8>(numero);
}
