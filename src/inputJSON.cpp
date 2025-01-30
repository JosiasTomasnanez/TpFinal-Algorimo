
#include "inputJSON.h"
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using namespace std;

void configPorConsola(string path) {
  int cantidadRouters;
  int verificacion;
  // Solicitar al usuario la cantidad de routers
  cout << "ingrese 1 si quiere usar el json ya definido, sino ingrese otro "
          "numero para configurar el JSON."
       << endl;
  cin >> verificacion;
  if (verificacion == 1) {
    return;
  }
  ofstream arch(path);
  if (!arch.is_open()) {
    cerr << "No se pudo abrir el archivo: " << endl;
    exit(1);
  }
  cout << "Ingrese la cantidad de routers: ";
  cin >> cantidadRouters;

  // Validar la cantidad de routers
  if (cantidadRouters <= 0) {
    cerr << "La cantidad de routers debe ser mayor a 0." << endl;
    return;
  }

  // Crear dinámicamente la matriz de ancho de banda
  int **anchoDeBanda = new int *[cantidadRouters];
  for (int i = 0; i < cantidadRouters; ++i) {
    anchoDeBanda[i] = new int[cantidadRouters];
  }

  // Solicitar los valores de la matriz de ancho de banda
  cout << "Ingrese los valores de la matriz de ancho de banda ("
       << cantidadRouters << "x" << cantidadRouters
       << ") (usa -1 si no hay conexión):" << endl;

  for (int i = 0; i < cantidadRouters; ++i) {
    for (int j = 0; j < cantidadRouters; ++j) {
      cout << "Ancho de banda [" << i << "][" << j << "]: ";
      cin >> anchoDeBanda[i][j];
    }
  }

  // Crear dinámicamente el arreglo de ancho_de_banda_terminales
  int *anchoDeBandaTerminales = new int[cantidadRouters];
  cout << "Ingrese los valores de ancho_de_banda_terminales:" << endl;
  for (int i = 0; i < cantidadRouters; ++i) {
    cout << "Ancho de banda para la terminal " << i << ": ";
    cin >> anchoDeBandaTerminales[i];
  }

  // Crear dinámicamente el arreglo de numero_de_terminales
  int *numeroDeTerminales = new int[cantidadRouters];
  cout << "Ingrese los valores de numero_de_terminales:" << endl;
  for (int i = 0; i < cantidadRouters; ++i) {
    cout << "Número de terminales para el router " << i << ": ";
    cin >> numeroDeTerminales[i];
  }

  // Crear el JSON
  json j;
  j["cantidad_routers"] = cantidadRouters;

  // Convertir la matriz de ancho de banda en un arreglo JSON
  for (int i = 0; i < cantidadRouters; ++i) {
    json fila;
    for (int j = 0; j < cantidadRouters; ++j) {
      fila.push_back(anchoDeBanda[i][j]);
    }
    j["ancho_de_banda"].push_back(fila);
  }

  // Agregar ancho_de_banda_terminales y numero_de_terminales al JSON
  for (int i = 0; i < cantidadRouters; ++i) {
    j["ancho_de_banda_terminales"].push_back(anchoDeBandaTerminales[i]);
    j["numero_de_terminales"].push_back(numeroDeTerminales[i]);
  }

  arch << j.dump(); // Escribir con sangría de 4 espacios
  cout << "Archivo JSON modificado / generado con éxito: config.json" << endl;

  // Liberar memoria dinámica
  for (int i = 0; i < cantidadRouters; ++i) {
    delete[] anchoDeBanda[i];
  }
  delete[] anchoDeBanda;
  delete[] anchoDeBandaTerminales;
  delete[] numeroDeTerminales;
  arch.close();
}