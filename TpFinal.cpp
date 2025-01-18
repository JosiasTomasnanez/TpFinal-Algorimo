#include <bitset>
#include <cstddef>
#include <iostream>
#include <vector>
using namespace std;

static int idPag=0;

struct Paquete{
int destino;
int origen;
int tamano;
int numeroPaquete;
int id;
int totalPaquete;
};

struct Pagina{
int destino;
int origen;
int tamano;
int id;


};


/**
 * @brief Clase Nodo que representa un nodo genérico para una estructura de
 * datos.
 *
 * @tparam T Tipo de dato almacenado en el nodo.
 */
template <class T> class Nodo {
private:
 
  T dato;        ///< Dato almacenado en el nodo.
  Nodo *next;    ///< Puntero al siguiente nodo.

public:
  Nodo() {
    next = NULL;
  };
  Nodo(T a) {
    dato = a;
    next = NULL;
  };
 
  void set_dato(T a) { dato = a; };
  void set_next(Nodo *n) { next = n; };
  T get_dato(void) { return dato; };
  Nodo *get_next(void) { return next; };
  bool es_vacio(void) { return next == NULL; };
};

template <class Paquete> class NodoArbol {
private:
  vector<Paquete> paquetes;
  int id; 
  NodoArbol<Paquete>*derecha;
  NodoArbol<Paquete>*izquierda;
public:
  NodoArbol() {
   derecha=nullptr; izquierda=nullptr;
  };
  Pagina* add_paquete(Paquete p){paquetes.push_back(p);
                              if(paquetes.size()== p->totalPaquete) return armarPagina(paquetes);
                              return nullptr;};
  Pagina* armarPagina(vector<Paquete> paquetes);
  void ordenarPaquetes();
  void swap();
  void set_id(int d){id=d;};
  int get_id(){return id;};
  NodoArbol<Paquete> *&get_der(void) { return derecha; };
  void set_der(NodoArbol<Paquete> *n) { derecha = n; };
  NodoArbol<Paquete> *&get_izq() { return izquierda; };
  void set_izq(NodoArbol<Paquete> *n) { izquierda = n; };
};

template <class Paquete> class ArbolPaquetes {
private:
  NodoArbol<Paquete> *raiz; ///< Puntero a la raíz del árbol.
  NodoArbol<Paquete> *q;
  Pagina* ArbolBusq(Paquete x, NodoArbol<Paquete>*& nuevo);
  void bor(NodoArbol<Paquete>*& d);
  void borrar(NodoArbol<Paquete>*& p, NodoArbol<Paquete> nodo );
  void eliminarRecursivo(NodoArbol<Paquete> *nodo) {
    if (nodo != nullptr) {
      eliminarRecursivo(nodo->get_izq());
      eliminarRecursivo(nodo->get_der());
      delete nodo;
    }
  }

public:
  ArbolPaquetes() {
  };
  ~ArbolPaquetes<Paquete>() { eliminarRecursivo(raiz); };
  Pagina* CreaArbolBus(Paquete p){ArbolBusq(p, raiz);};
  void Borrar(NodoArbol<Paquete> nodo) { borrar(raiz, nodo); }
};

template <class Paquete> Pagina* ArbolPaquetes<Paquete>::ArbolBusq(Paquete p, NodoArbol<Paquete>*& nuevo)
{
    if (nuevo == NULL) {
        nuevo = new NodoArbol<Paquete>;
        nuevo->set_id(p->id); nuevo->der = nuevo->izq = NULL; Pagina* aux= nuevo->add_paquete(p);
        if(aux!=nullptr){borrarNodo(nuevo);}
        return aux
    }
    if(nuevo->get_id()== p.get_id()){Pagina*aux= nuevo->add_paquete(p);
                                     if(aux!=nullptr){borrarNodo(nuevo);}return aux;}
    if (p->get_id() > nuevo->get_id()) ArbolBusq(p, nuevo->get_der());
    if (p->get_id < nuevo->get_id()) ArbolBusq(p, nuevo->get_izq());
}
template <class Paquete> void ArbolPaquetes<Paquete>::borrar(NodoArbol<Paquete>*& p,NodoArbol<Paquete> nodo)
{
    if (p == NULL) cout << "\n El dato NO esta\n\n";
    else if (nodo.get_id() > p->get_id()) borrar(p->get_der(), nodo);
    else if (nodo.get_id()< p->get_id()) borrar(p->get_izq(), nodo);
    else {// lo encontre en el nodo p
        q = p;
        if (q->get_der() == NULL) p = q->get_izq();// raiz<=raiz del subarbol izq
        else if (q->get_izq() == NULL) p = q->get_der();//raiz<=raiz del subarbol der
        else bor(q->get_izq);//busca en el sub arbol izq
        delete q;
    }
}
template <class Paquete> void ArbolPaquetes<Paquete>::bor(NodoArbol<Paquete>*& d)
{
    if (d->get_der() != nullptr) bor(d->get_der());//busca el elemento mas a la derecha
    else {
        q->get_id()= d->get_id();
        q = d;
        d = d->get_izq();
    }
}


template <class T> class Cola {
private:
  Nodo<T> *czo;    

public:
  Cola() {
    czo = new Nodo<T>();
  };
  Cola(Nodo<T> *n) {
    czo = n;
  };
  ~Cola() {
    Nodo<T> *current = czo;
    while (current != nullptr) {
      Nodo<T> *next = current->get_next();
      delete current; // Libera la memoria del nodo
      current = next;
    }
  };
  void add(T d);
  bool esvacia(void);
  Nodo<T> *nodoCabeza(void); // retorna el dato del primer nodo
  Cola *
  resto(void); // retorna el puntero al "resto" de la lista
  void impre();
};





class Router{
     private:

    ArbolPaquetes<Paquete>* gestorPaquetes;

    public:
   Router(){gestorPaquetes=new ArbolPaquetes<Paquete>();};
   ~Router(){delete gestorPaquetes;};
   void recibirDeTerminal(Pagina pag);
   void recibir(Paquete paq);
   void guardarPaquete(Paquete paq);
   bool verificarDestino();//analizar que hacer con los paquetes , no se si esta bien esto, , como poner en lista de algun vecino
   void separarPagina();
   void ejecutarCiclo();//podriamos crear maquina de estados para ver que toca hacer cuando sea ciclo, como poner en lista de algun vecino
};

void Router::guardarPaquete(Paquete paq){
Pagina* p= gestorPaquetes->CreaArbolBus(paq);
if(p!=nullptr){
enviar();//o que quede listo para enviar a la terminal destino de la pagina
}
}


void Router::recibirDeTerminal(Pagina pag){
    separarPagina();
    if(verificarDestino()){return;}
}

static Pagina generarPagina(){
//generacion aleatoria, menos el id
}

class Terminal{
 private:
   Router* routerAsociado;
   Cola<Pagina>* paginasRecibidas;

    public:
    Terminal(Router* r){routerAsociado=r;};
    ~Terminal(){delete routerAsociado;};
    void recibirPaginas(Pagina p){paginasRecibidas->add(p);};
    void  generarPag(){routerAsociado->recibirDeTerminal(generarPagina());};
};

class AdministradorDelSistema{
 private:



    public:
};


class Ciclo{ 
    private:



    public:
};


class Simulador{

     private:



    public:

};




int main(){

    cout<< "";
}
