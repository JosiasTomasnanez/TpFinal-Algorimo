# Versión mínima de CMake
cmake_minimum_required(VERSION 3.10)

# Establece el tipo de compilación predeterminado como Debug
set(CMAKE_BUILD_TYPE Debug)

# Opciones específicas para Debug
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")

# Nombre del proyecto
project(MiProyecto)

# Define las opciones de compilación (por ejemplo, C++11)
set(CMAKE_CXX_STANDARD 11)

# Agrega el submódulo de la librería nlohmann/json
add_subdirectory(submodulos/jsonLib)

set(SOURCES
    src/Dijkstra.cpp
    src/TpFinal.cpp
    src/inputJSON.cpp
)

# Encuentra todos los archivos .h en la carpeta include
include_directories(${CMAKE_SOURCE_DIR}/include)

# Agrega el ejecutable a partir de los archivos .cpp encontrados
add_executable(sim_internet_red ${SOURCES})

# Enlaza el ejecutable con la librería nlohmann/json (si es necesario)
target_link_libraries(SimuladorRed nlohmann_json)

# Establece el directorio de salida del binario
set_target_properties(sim_internet_red PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
)

