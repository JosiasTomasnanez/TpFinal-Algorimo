class Router;

template <typename T>
class Cola;
struct Pagina;
class Terminal{
 private:
   Router* routerAsociado;
   Cola<Pagina*>* paginasRecibidas;
   int ocupacionPorCiclo;
   int  id;
    
    public:
    Terminal(Router* r, int id);
    ~Terminal();
    
    int getOcupacion(){return ocupacionPorCiclo;};
    void setOcupacion(int ocupacion){ocupacionPorCiclo=ocupacion;};
    int getId(){return id;};
    
    void recibirPaginas(Pagina* p);
    void generarPag();
};

