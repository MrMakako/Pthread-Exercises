Si estas usando CLion, modifica el nombre del proyecto que le pusiste en el CMake.

El CMake ya trae incluido todo lo que ocupa para usar omp.




// ejemplo -- donde dice nombreDeTuProyecto pones el nombre del proyecto

cmake_minimum_required(VERSION 3.23)
project(nombreDeTuProyecto)

set(CMAKE_CXX_STANDARD 14)

find_package(OpenMP REQUIRED)

add_executable(nombreDeTuProyecto main.cpp)
target_link_libraries(nombreDeTuProyecto PRIVATE OpenMP::OpenMP_CXX)


Nota:
Tambien cambia las rutas de los archivos de donde los hilos productores van a leer.

Asegurate de que la ruta concuerde con el lugar en donde guardaste la coleccion de archivos.