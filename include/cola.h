#include <iostream>
using namespace std;
/**
 * @brief Clase Nodo que representa un nodo genérico para una estructura de
 * datos.
 *
 * @tparam T Tipo de dato almacenado en el nodo.
 */
template <class T> class Nodo {
private:
  T dato;     ///< Dato almacenado en el nodo.
  Nodo *next; ///< Puntero al siguiente nodo.

public:
  Nodo() { next = nullptr; };
  Nodo(T a) {
    dato = a;
    next = nullptr;
  };

  void set_dato(T a) { dato = a; };
  void set_next(Nodo *n) { next = n; };
  T get_dato(void) { return dato; };
  Nodo *get_next(void) { return next; };
  bool es_vacio(void) { return next == nullptr; };
};
template <class T> class Lista {
private:
  void borrarD(T d, Nodo<T> *ant);

public:
  Lista() { czo = new Nodo<T>(); };
  Lista(Nodo<T> *n) { czo = n; };
  //~Lista(void);
  void add(T d); // sumar nodos a la lista
  bool esvacia(void);
  T cabeza(void);     // retorna el dato del primer nodo
  Lista *resto(void); // retorna el puntero al "resto" de la lista
                      // resto= lo que queda de la lista sin la cabeza
  void impre(void);
  int size();

  T get(int pos);
  T last();
  bool contains(T d);
  void borrarDato(T d) { borrarD(d, nullptr); } // borra el nodo que contiene d
  void borrar(void);                            // borra la cabeza
  void borrar_last();                           // borra el ultimo
  T buscar(T d);

protected:
  Nodo<T> *czo;
};
template <class T> class Cola : public Lista<T> {
public:
  Cola(void) { Lista<T>(); };
  Cola(Nodo<T> *n) : Lista<T>(n) {}
  T tope() { return this->last(); };
  bool colavacia(void) { return this->esvacia(); };
  void encolar(T a) { this->add(a); };
  void desencolar(void) { this->borrar_last(); };
  T ultimo(void) { return this->cabeza(); };
  Cola<T> *restoCola();
  void concat(Cola<T> *l1);
  Cola<T> *copy(void);
};
template <class T> void Lista<T>::add(T d) {
  Nodo<T> *nuevo = new Nodo<T>(d);
  nuevo->set_next(czo);
  czo = nuevo;
}
template <class T> T Lista<T>::get(int pos) {
  if (pos < 0 || this->esvacia()) {
    return T(); // Retorna valor por defecto si la posición es inválida
  }
  if (pos == 0) {
    return this
        ->cabeza(); // Retorna el dato del nodo actual si es la posición buscada
  }
  return this->resto()->get(
      pos - 1); // Avanza al siguiente nodo y disminuye la posición
}

template <class T> bool Lista<T>::esvacia(void) { return czo->es_vacio(); }
template <class T> T Lista<T>::cabeza(void) {
  if (this->esvacia()) {
    return T();
  }
  return czo->get_dato();
}

template <class T> Lista<T> *Lista<T>::resto(void) {
  Lista *l = new Lista(czo->get_next());
  return (l);
}
template <class T> Cola<T> *Cola<T>::restoCola(void) {
  Cola *c = new Cola(this->czo->get_next());
  return (c);
}
template <class T> T Lista<T>::last() {
  if (!this->esvacia()) {
    if (this->resto()->esvacia())
      return this->cabeza();
    return this->resto()->last();
  }
  return T();
}
template <class T>
void Cola<T>::concat(Cola<T> *l1) { // le transfiere los datos de l1 a this
  if (!(l1->esvacia())) {
    this->concat(l1->restoCola());
    this->add(l1->cabeza());
  }
}
template <class T> bool Lista<T>::contains(T d) {
  Nodo<T> *aux = czo; // Puntero auxiliar para recorrer la lista
  while (aux != nullptr) {
    if (aux->get_dato() == d) { // Compara el dato actual con el buscado
      return true;              // Retorna true si encuentra el elemento
    }
    aux = aux->get_next(); // Avanza al siguiente nodo
  }
  return false; // Retorna false si no se encuentra el elemento
}

template <class T> Cola<T> *Cola<T>::copy(void) {
  Cola<T> *aux = new Cola();
  aux->concat(this);
  return aux;
}

template <class T> int Lista<T>::size() {
  if (this->esvacia())
    return 0;
  return 1 + this->resto()->size();
}

template <class T> void Lista<T>::borrar(void) { // borra el nodo cabeza
  if (!this->esvacia()) {
    Nodo<T> *tmp = czo;
    czo = czo->get_next();
    delete tmp;
  }
}

template <class T> void Lista<T>::impre(void) {
  Nodo<T> *aux;
  aux = czo;
  while (aux->get_next() != NULL) {
    cout << aux->get_dato() << endl;
    aux = aux->get_next();
  }
}
template <class T> void Lista<T>::borrar_last() { // borra el ultimo nodo
  if (!this->esvacia()) {
    if ((czo->get_next())->get_next() == NULL) {
      delete czo->get_next();
      czo->set_next(NULL);
    } else
      this->resto()->borrar_last();
  }
}

template <class T> void Lista<T>::borrarD(T d, Nodo<T> *ant) {
  if (!this->esvacia()) {
    if (d == this->cabeza()) {
      if (ant == NULL) { // al principio
        this->borrar();
      } else { // entre medio
        ant->set_next(czo->get_next());
        delete czo;
      }
    } else
      this->resto()->borrarD(d, czo);
  }
}
