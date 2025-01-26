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
    
    int getId(){return id;};
    int getOcupacion(){return ocupacionPorCiclo;};
    void setOcupacion(int ocupacion){ocupacionPorCiclo=ocupacion;};
    
    void recibirPaginas(Pagina* p);
    void generarPag();
};

