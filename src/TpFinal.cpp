#include <bitset>
#include <climits>
#include <cstddef>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>
#include <fstream>
#include "terminal.h"
#include <unistd.h> 
#include "structs.h"
#include "cola.h"
#include "arbolBinario.h"
#include "Dijkstra.h"
#include <nlohmann/json.hpp>
#include "inputJSON.h"

using namespace std;
using json = nlohmann::json;

#define INFI INT_MAX
#define LINE_SIZE 256

ofstream logfile("../log.txt");
int tamañoPaquete=4;
int idPag=0;

class ColadeConecciones : public Cola<coneccionVecino> {
private:
template <class T>
ColadeConecciones* restoC() {
    if (this->czo == nullptr) {
        return nullptr;
    }
    Nodo<coneccionVecino>* siguiente = this->czo->get_next();
    ColadeConecciones* nuevaCola = new ColadeConecciones();
    nuevaCola->czo = siguiente;
    return nuevaCola;
}

public:
coneccionVecino* buscarConeccion(bitset<16> ip){//recorrer y comparar el ip de cada puntero a router

coneccionVecino aux= cabeza();
coneccionVecino* auxcabeza= &aux;
if(auxcabeza->colaEnvio==nullptr){return nullptr;}
if(auxcabeza->ipVecino==ip){
  return auxcabeza;
}
 // Si no es el nodo que buscamos, llama recursivamente al resto de la cola
    ColadeConecciones* nuevaCola = restoC<coneccionVecino>();  // Obtener el resto de la cola sin el primer nodo
    if (nuevaCola == nullptr) {
        return nullptr;  // No encontró el vecino en la cola restante
    }

    // Llamada recursiva al siguiente nodo
    return nuevaCola->buscarConeccion(ip);
}
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
                              if(paquetes.size()== p.totalPaquete) return armarPagina(paquetes);
                              return nullptr;};
  Pagina* armarPagina(vector<Paquete>& paquetes);
  void ordenarPaquetes(vector<Paquete>& paquetes, int primero, int ultimo);
  void swap(vector<Paquete>& paquetes, int pos1, int pos2);
  void set_id(int d){id=d;};
  int get_id(){return id;};
  NodoArbol<Paquete> *&get_der(void) { return derecha; };
  void set_der(NodoArbol<Paquete> *n) { derecha = n; };
  NodoArbol<Paquete> *&get_izq() { return izquierda; };
  void set_izq(NodoArbol<Paquete> *n) { izquierda = n; };
};
template <class Paquete>
 Pagina* NodoArbol <Paquete>::armarPagina(vector<Paquete>& paquetes){
    
    Pagina* aux= new Pagina;
    aux->destino=paquetes[0].destino;
    aux->origen=paquetes[0].origen;
    aux->id=paquetes[0].id;
    aux->datopagina="";
    //aux.tamano=paquetes[0].tamano;          tamano en paquetes es el tamaño total de la pagina?
    ordenarPaquetes(paquetes, 0, paquetes.size()-1);
    for(int i=0; i<paquetes.size(); i++){
        aux->datopagina= aux->datopagina+paquetes[i].datopaquete;
    }
    
    return aux;
}


template <class Paquete>
 void NodoArbol <Paquete>::ordenarPaquetes(vector<Paquete>& paquetes, int primero, int ultimo){
    if (ultimo > primero) {
        Paquete pivote= paquetes[ultimo];
        int i = primero - 1;
        int j = ultimo;
        while (true) {

            while (paquetes[++i].numeroPaquete<pivote.numeroPaquete);
                
            while (paquetes[--j].numeroPaquete>pivote.numeroPaquete);

            if(i>=j)break;
            swap(paquetes, i, j);
        }

        swap(paquetes, i, ultimo);
        ordenarPaquetes(paquetes, primero, i-1);
        ordenarPaquetes(paquetes, i + 1, ultimo);       
        
        }

}

template <class Paquete>
 void NodoArbol <Paquete>::swap(vector<Paquete>& paquetes, int pos1, int pos2){
    
    Paquete paqueteaux=paquetes[pos1];
    paquetes[pos1]=paquetes[pos2];
    paquetes[pos2]=paqueteaux;

 }

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
  ArbolPaquetes() { raiz=nullptr;
  };
  ~ArbolPaquetes<Paquete>() { eliminarRecursivo(raiz); };
  Pagina* CreaArbolBus(Paquete p){return ArbolBusq(p, raiz);};
  void Borrar(NodoArbol<Paquete> nodo) { borrar(raiz, nodo); }
};


template <class Paquete> Pagina* ArbolPaquetes<Paquete>::ArbolBusq(Paquete p, NodoArbol<Paquete>*& nuevo)
{
    if (nuevo == nullptr) {
        nuevo = new NodoArbol<Paquete>;
        nuevo->set_id(p.id); nuevo->get_der() = nuevo->get_izq() = nullptr; Pagina* aux= nuevo->add_paquete(p);
        if(aux!=nullptr){Borrar(*nuevo);}
        return aux;
    }
    if(nuevo->get_id()== p.id){Pagina*aux= nuevo->add_paquete(p);
                                     if(aux!=nullptr){Borrar(*nuevo);}
                                     return aux;}
    if (p.id > nuevo->get_id()) return ArbolBusq(p, nuevo->get_der());
    if (p.id < nuevo->get_id()) return ArbolBusq(p, nuevo->get_izq());
    return nullptr;
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
        else bor(q->get_izq());//busca en el sub arbol izq
        delete q;
    }
}
template <class Paquete> void ArbolPaquetes<Paquete>::bor(NodoArbol<Paquete>*& d)
{
    if (d->get_der() != nullptr) bor(d->get_der());//busca el elemento mas a la derecha
    else {
        q->set_id(d->get_id());
        q = d;
        d = d->get_izq();
    }
}


class Router{
     private:
        ArbolBinario1Byte<coneccionTerminal>* terminales;
        ArbolPaquetes<Paquete>* gestorPaquetes;
        ColadeConecciones*routerVecinos;
        Dijkstra* d; 
        Cola<Paquete> paquetePerdidos;
        int ocupacionPorCiclo;
        int n;
        int** tablaDestinos;//hacer otra tabla con los valores prefijados, a cada nodo  poniendo a quien le tiene que mandar 
                            //el paquete , cosa de solo recalcular cada 2 ciclos
        int* cantTerminales;
        bitset<16> routerIp;
        int anchoBandaT;
        Cola< Paquete> paquetesParaAnalizar;
        void comprobarMejorCamino(Paquete paq, int origen, int destino);
        void separarPagina(Pagina pag);
        void crearTablaDestino();
      public:
        Router(int n){gestorPaquetes=new ArbolPaquetes<Paquete>();terminales= new ArbolBinario1Byte<coneccionTerminal>(); this->n=n;};
        ~Router(){delete gestorPaquetes;};

        Cola<Paquete> getPaquetesPerdidos(){return paquetePerdidos;}
        int getNumRouters() {return n;};
        int* getNumTerminaales(){return cantTerminales;};
        int getOcupacion(){return ocupacionPorCiclo;};
        void setOcupacion(int ocupacion){ocupacionPorCiclo=ocupacion;};
        void generarTerminales(int numTerminales){
            for(int i=0;i<numTerminales;i++){
                coneccionTerminal cone;
                cone.terminal= new Terminal(this,i);
                cone.anchoDeBanda=anchoBandaT;//ramdom
                terminales->addArbol(cone);
      
                 }
            };
        void config(int** tablaPesos){d=new Dijkstra(n,tablaPesos); crearTablaDestino();};
        void actualizarTablaDestino();
        void setVecinos(ColadeConecciones* c){routerVecinos=c;};
        void set_IP(bitset<16> b){routerIp= b;}
        void setNumTerminales(int* nt){cantTerminales=nt;};
        void recibirDeTerminal(Pagina pag);
        ColadeConecciones* get_vecinos(){return routerVecinos;}
        int** getTabla(){return tablaDestinos;};
        void recibir(Paquete paq);
        void setAnchoBanda(int ab){anchoBandaT=ab;}
        void guardarPaquete(Paquete paq);
        void ejecutarCiclo();//podriamos crear maquina de estados para ver que toca hacer cuando sea ciclo, como poner en lista de algun vecino
        bitset<16> get_ip(){return routerIp;};
};

 void  Router::ejecutarCiclo(){
    for(int i=0; i < paquetesParaAnalizar.size();i++){
        Paquete p = paquetesParaAnalizar.get(i);
        coneccionTerminal t= terminales->buscarDato(p.destino);
        if(t.anchoDeBanda-t.terminal->getOcupacion()>= p.tamano){
           guardarPaquete(p);
           t.terminal->setOcupacion(t.terminal->getOcupacion()-p.tamano);
        }
    }
    for (int j=0; j< terminales->size();j++) {
     
     terminales->get( j).terminal->setOcupacion(0);
    }

   for(int i=0; i< routerVecinos->size(); i++){
    coneccionVecino p=routerVecinos->get(i);
     if(p.colaEnvio->size()!=0){
        Cola<Paquete>* cp= p.colaEnvio;
     for(int j=0; j< cp->size();j++){
        Paquete paq= cp->get(j);
        if(p.anchoDeBanda-p.vecino->getOcupacion()>=paq.tamano ){ 
            p.vecino->recibir(paq);
            p.vecino->setOcupacion(p.vecino->getOcupacion()-paq.tamano);
        }
     }
         p.vecino->setOcupacion(0);
     }
   }
  for (int j=0; j< terminales->size();j++) {
     terminales->get( j).terminal->generarPag();
   
    }
 }

void Router::crearTablaDestino(){
tablaDestinos = new int*[n];
for(int i=0;i<n;i++){
    tablaDestinos[i] = new int[n];
    for(int j=0; j<n; j++){
        tablaDestinos[i][j]= d->dijkstra(i, j);
    }
}
}

void Router::actualizarTablaDestino(){
for(int i=0;i<n;i++){
    for(int j=0; j<n; j++){
        tablaDestinos[i][j]= d->dijkstra(i, j);
    }
}
}

void Router::recibir(Paquete paq){
     bitset<8> aux1 = (paq.destino.to_ulong() >> 8) & 0xFF;  // Desplazamos y aplicamos la máscara para los primeros 8 bits
     bitset<8> aux2= (routerIp.to_ulong() >> 8) & 0xFF; 
  if(aux1==aux2){
    paquetesParaAnalizar.add(paq);
    return;
  }
 coneccionVecino* con=routerVecinos->buscarConeccion(paq.destino);
 if(con!=nullptr){
    con->colaEnvio->add(paq);
    return;
 }

 int origen=static_cast<int>(aux2.to_ulong());
 int destino =static_cast<int>(aux1.to_ulong());
comprobarMejorCamino(paq, origen,destino);
}

void Router::comprobarMejorCamino(Paquete paq, int  origen, int  destino){
 if(tablaDestinos[origen][destino]==-1){
    paquetePerdidos.add(paq);
    return;
 }
 bitset<16>aux;
 aux=tablaDestinos[origen][destino]<<8;
 coneccionVecino* con=routerVecinos->buscarConeccion(aux);
 if(con!=nullptr){
    con->colaEnvio->add(paq);
 }
 
}


void Router:: separarPagina(Pagina pag){

   for(int i=0; i<pag.tamano/tamañoPaquete  ; i++){
    if(i==(pag.tamano/tamañoPaquete) && pag.tamano%tamañoPaquete==0){break;}
        Paquete p ;
        p.numeroPaquete=i+1;
        p.totalPaquete =(pag.tamano / tamañoPaquete)+1;
        p.destino=pag.destino;
        p.origen=pag.origen;
        p.id=pag.id;
        p.tamano=pag.tamano;
       recibir(p);
   }
}

void Router::guardarPaquete(Paquete paq){
Pagina* p= gestorPaquetes->CreaArbolBus(paq);
if(p!=nullptr) terminales->buscarDato(p->destino).terminal->recibirPaginas(p);
}

void Router::recibirDeTerminal(Pagina pag){
    separarPagina(pag);
}

static Pagina generarPagina(int* numTerminales, int numRouters, int idTerminal, bitset<16> ipRouter) {
    Pagina nuevaPagina;
    // Calcular el origen: los primeros 8 bits son el router (ipRouter.to_ulong() >> 8),
    // y los últimos 8 bits son el idTerminal
    bitset<16> origen = ipRouter;
    origen &= bitset<16>(0xFF00);  // Mantener solo los bits del router
    origen |= bitset<16>(idTerminal);  // Agregar los bits de la terminal
    nuevaPagina.origen = origen;
    // Generar destino aleatorio
    int randomRouter = rand() % numRouters;  // Elegir un router aleatorio
    int randomTerminal = rand() % numTerminales[randomRouter];  // Elegir una terminal aleatoria dentro del router
    bitset<16> destino = (bitset<16>(randomRouter) << 8) | bitset<16>(randomTerminal);  // Combinar router y terminal
    nuevaPagina.destino = destino;
 
    nuevaPagina.tamano = 5 + (rand() % 11);
    // Asignar ID único
    nuevaPagina.id = idPag++;
    // Ignorar datopagina
    nuevaPagina.datopagina = "";
    return nuevaPagina;
}


/************************************************************************************************
******************METODOS TERMINAL***************************************************************
*/
// Constructor
Terminal::Terminal(Router* r,int idT) : 
        routerAsociado(r), paginasRecibidas(new Cola<Pagina*>()), ocupacionPorCiclo(0) ,id(idT){}

// Destructor
Terminal::~Terminal() {
    delete routerAsociado;    // Asegúrate de que es seguro eliminarlo
    delete paginasRecibidas; // Libera memoria de la cola
}

// Método para recibir páginas
void Terminal::recibirPaginas(Pagina* p) {
    if (paginasRecibidas) {
        paginasRecibidas->add(p);
    }
}

// Método para generar y enviar páginas al router
void Terminal::generarPag() {
    
        int random= rand()%2;
        for(int i=0;i<random;i++){ 
        routerAsociado->recibirDeTerminal(generarPagina(routerAsociado->getNumTerminaales(),
                                            routerAsociado->getNumRouters(),id,routerAsociado->get_ip())); 
        }
    
}
/************************************************************************************************
*************************************************************************************************
*/



class AdministradorDelSistema{
 private:
    int ** matrizPesos;
    int ** matrizAdyacencia;
    int  contadorCiclo;

    void actualizarMatrizPesos();
    void recalcularPesos();
    void ciclo();
    void generarMatriz(int** ma, int n);
    Cola<Router*> routers;
    void generarRouters(int** anchoBanda,int n,int* mt,int* mnt);
    int n;
 public:
    AdministradorDelSistema( int n,int** anchoBanda,int* mt,int* mnt){this->n=n;matrizAdyacencia=anchoBanda; 
                            generarMatriz(anchoBanda,n);generarRouters(anchoBanda,n,mt,mnt);contadorCiclo=0;};
    
  void initSim(int ciclosMax){for(int i=0;i<ciclosMax;i++){ciclo(); cout<< "ciclo ejecutado " << i<<endl<<flush;}};

};

void AdministradorDelSistema::ciclo(){
   if(contadorCiclo>=1){
    recalcularPesos();
    for(int j=0;j<routers.size();j++){
        routers.get(j)->actualizarTablaDestino();
    }
    contadorCiclo=-1;
   }

for(int i = 0; i < n; i++){
        routers.get(i)->ejecutarCiclo();
    }
    contadorCiclo++;
}


 void AdministradorDelSistema::recalcularPesos(){
    for(int i = 0; i < n; i++){
        ColadeConecciones* colcon= routers.get(i)->get_vecinos();
        for (int j = 0; j < n; j++){
            if(i==j){
                matrizPesos[i][j]=0;
                continue; 
            }
            coneccionVecino* con = colcon->buscarConeccion(routers.get(j)->get_ip());
            if(con!=nullptr){
                int sizeTotal=con->colaEnvio->size()*tamañoPaquete;
                int anchoBanda=con->anchoDeBanda;
                if(sizeTotal%anchoBanda !=0){
                     matrizPesos[i][j]= (sizeTotal/anchoBanda)+1;
                }else {
                     matrizPesos[i][j]= sizeTotal/anchoBanda;
                }
               
            }
        }
    }


 }


void AdministradorDelSistema::generarRouters(int** anchoBanda,int n,int* mt,int* mnt){
for (int i = 0; i < n; i++) {
    bitset<16> b= i<<8;
    Router* r = new Router(n);
    r->set_IP(b);
    r->setAnchoBanda(mt[i]);
    r->generarTerminales(mnt[i]);
    r->config(matrizPesos);
    r->setNumTerminales(mnt);
    routers.add(r);
    
}

for (int i = 0; i < n; i++) {
    ColadeConecciones* c= new ColadeConecciones();
    Router* rou_i=routers.get(i);
    for (int j = 0; j < n; j++) {
        if(matrizPesos[i][j]==1 && i!=j){
            Router* rou_j=routers.get(j);
            coneccionVecino con;
            con.ipVecino= rou_j->get_ip();
            con.vecino= rou_j;
            con.anchoDeBanda= anchoBanda[i][j];
            con.colaEnvio= new Cola<Paquete>();
            c->add(con);
           
        }
       
    }
    rou_i->setVecinos(c);
}
}

void AdministradorDelSistema::generarMatriz(int ** ma, int n){
     
      matrizPesos=new int*[n];
     for (int i = 0; i < n; i++) {
          matrizPesos[i] = new int[n];
        for (int j = 0; j < n; j++) {
            if(i==j){
                matrizPesos[i][j]=0;
                continue;
            }
           if(ma[i][j]!=INT_MAX){
            matrizPesos[i][j]=1;
           }
           else {
           matrizPesos[i][j]=INT_MAX;
           }
           
          
        }
     }
}





int main(){

logfile.is_open();



  char cwd[LINE_SIZE];
    if (!getcwd(cwd, sizeof(cwd))) {
        cerr << "No se pudo obtener el directorio actual" << endl;
        return 1;
    }

    string currentDir(cwd);

    // Retrocede un nivel eliminando la última parte del directorio
    size_t lastSlash = currentDir.find_last_of('/');
    if (lastSlash != string::npos) {
        currentDir = currentDir.substr(0, lastSlash); // Elimina la parte final
    }

    // Construye la ruta al archivo
    string projectRoot = currentDir + "/jsonfiles/config.json";

    // Intenta abrir el archivo
    ifstream archivo(projectRoot);

      configPorConsola(projectRoot); 

    if (!archivo.is_open()) {
      cerr << "No se pudo abrir el archivo: " << projectRoot << endl;
       exit(1);
    } 


    json config;
    archivo >> config;

    int cantidadRouters = config["cantidad_routers"];

    // Crear y rellenar la matriz de adyacencia para routers
    
    int** matrizAdyacencia = new int*[cantidadRouters];
    for (int i = 0; i < cantidadRouters; ++i) {
        matrizAdyacencia[i] = new int[cantidadRouters];
        for (int j = 0; j < cantidadRouters; ++j) {
            int anchoDeBanda = config["ancho_de_banda"][i][j];
            matrizAdyacencia[i][j] = (anchoDeBanda == -1 ) ? INT_MAX : anchoDeBanda;
        }
    }
    int* anchoBandaTerminales = new int[cantidadRouters];
        for (int i= 0; i < cantidadRouters; ++i) {
           anchoBandaTerminales[i] = config["ancho_de_banda_terminales"][i];
        }
    
    int* numTerminales = new int[cantidadRouters];
        for (int i= 0; i < cantidadRouters; ++i) {
            numTerminales[i] = config["numero_de_terminales"][i];
        }
    

  AdministradorDelSistema admin(cantidadRouters,matrizAdyacencia,anchoBandaTerminales,numTerminales);
   admin.initSim(13);
   archivo.close();
   logfile.close();
    
    // Liberar memoria de la matriz de adyacencia
    for (int i = 0; i < cantidadRouters; ++i) {
        delete[] matrizAdyacencia[i];
    }
    delete[] matrizAdyacencia;

    return 0;
}

