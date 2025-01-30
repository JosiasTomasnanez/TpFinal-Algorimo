# Guía de instalación

Este documento proporciona instrucciones detalladas sobre cómo instalar y ejecutar el proyecto en sistemas Linux y Windows. Sigue los pasos correspondientes a tu sistema operativo.

## Instalación de CMake

Para su compilacion y gestion de la libreria de Json, es necesario disponer de cmake.

### En Linux

1. **Instalar CMake**: usando el gestor de paquetes. Ejecuta el siguiente comando:
```bash
   sudo apt update
   sudo apt install cmake
```

2. **Verificar instalación**: Después de la instalación, puedes verificar que CMake se haya instalado correctamente con: `cmake --version`.

### En Windows

1. **Descargar e instalar CMake**:
        Ve al sitio web oficial de CMake: https://cmake.org/download/.
        Descarga el instalador de Windows y sigue las instrucciones del asistente de instalación.

2. **Agregar CMake al PATH**: Durante la instalación, asegúrate de seleccionar la opción que agrega    CMake al PATH, para poder ejecutarlo desde la terminal.

3. **Verificar instalación**: Abre el símbolo del sistema (cmd) y ejecuta: `cmake --version`.


# Instalación del programa de simulacion de red

1. **Ejecutar el script**: Puedes compilar el proyecto automáticamente utilizando el script compilador.sh: `./compilador.sh` o doble click en el mismo, y ejecutandolo con la terminal.
    **Una alternativa**: 
      Usar el comando `git submodule update --init --recursive` para clonar la libreria de json.
      Crear una carpeta "build" `mkdir build`, movernos dentro de ella ejecutando `cd build`
      una vez dentro ejecutar `cmake ..` y luego `make`, volvemos a la carpeta raiz
      `cd ..`, veremos que se creo la carpeta bin con el binario del programa.

2. **Ir a la carpeta bin**: Después de ejecutar el script, el archivo binario se generarán en la carpeta bin. Navega a esa carpeta: `cd bin`.

3. **Ejecutar el programa**: Ejecuta el binario del simulador (SimuladorRed): 
        En Linux:`./SimuladorRed`.
        En Windows: `SimuladorRed.exe`

