cmake_minimum_required(VERSION 3.23)
project(ejercicioOpenMP)

set(CMAKE_CXX_STANDARD 14)

find_package(OpenMP REQUIRED)

add_executable(ejercicioOpenMP main.cpp)
target_link_libraries(ejercicioOpenMP PRIVATE OpenMP::OpenMP_CXX)
