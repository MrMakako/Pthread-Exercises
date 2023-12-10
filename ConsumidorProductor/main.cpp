#include <iostream>
#include <fstream> // para manejar archivos
#include <vector> // para manejar las rutas de los archivos
#include <queue> // para guardar lo que hagan los productores
#include <string>
#include <omp.h> // omp para paralelizar
#include <sstream> // facilita el flujo de texto para poder hacer tokens

using namespace std;

// Cola para guardar data
queue<string> sharedQueue;

// Funcion que ejecutan los hilos productores
void producer(const string &filename, int idProductor) {
    // abrimos el archivo con ruta filename
    ifstream inputFile(filename);
    string line;

    // Zona critica -- un hilo a la vez guarda su linea en la cola
#pragma omp critical
    {
        // iteramos todo el archivo y extraemos cada linea
        while (getline(inputFile, line)) {
            printf("Producer #%d producing line... from %s\n", idProductor, filename.c_str());
            line += " idProductor:" + to_string(idProductor);
            sharedQueue.push(line);
        }
    };
}

// Funcion para hilos consumidores
void consumer() {
    string line;

    while (true) {

        if (sharedQueue.empty()) {
            break;
        }

        // Consigue la primera linea de la cola
        line = sharedQueue.front();
        sharedQueue.pop();

        // Tokenizamos una linea
        istringstream iss(line);
        string token;

        // Zona critica -- cuando los hilos tienen que imprimir su resultado
#pragma omp critical
        {
            printf("\nResult of consumer %d\n", omp_get_thread_num());
            while (iss >> token) {
                cout << "Consumer " << omp_get_thread_num() << ": " << token << endl;
            }
        };
    }
}

int main() {
    // Archivos para los hilos productores
    // Cambiar rutas para probar en local
    vector<string> filenames = {
            R"(file1.txt)",
            R"(file2.txt)",
            R"(file3.txt)",
    };

    // hilos productores
    // La clausula default es para decirle al compilador que no asuma nada
    // y que con la clausula shared le vamos a especificar explicitamente cuales son las variables compartidas
    // entre todos los hilos
#pragma omp parallel num_threads(filenames.size()) default(none) shared(filenames)
    {
        int threadId = omp_get_thread_num();
            producer(filenames[threadId], threadId);
    }

    //hilos consumidores
#pragma omp parallel num_threads(filenames.size()) default(none)
    {
        consumer();
    }

    return 0;
}
