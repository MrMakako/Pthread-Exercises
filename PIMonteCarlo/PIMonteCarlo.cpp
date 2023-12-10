#include <mpi.h>
#include <iostream>
#include <cstdlib>
#include <ctime>

/*
	Calcula el valor de Pi usando el m�todo de Monte Carlo.
	El m�todo de Monte Carlo consiste en generar n�meros aleatorios
	y determinar si estos n�meros se encuentran dentro de un c�rculo
	de radio 1. El valor de Pi se puede estimar como el n�mero de
	puntos dentro del c�rculo dividido entre el n�mero total de puntos
	lanzados.
*/

// La formula del metodo monte carlos es: pi = 4 * (puntos dentro del circulo / puntos totales)


int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    long long int number_of_tosses;
    if (world_rank == 0) {
        std::cout << "Ingrese el numero total de lanzamientos: ";
        std::cin >> number_of_tosses;
    }
    MPI_Bcast(&number_of_tosses, 1, MPI_LONG_LONG_INT, 0, MPI_COMM_WORLD);
    srand(time(NULL) + world_rank);  // Semilla para la generaci�n de n�meros aleatorios
    long long int number_in_circle = 0;
    for (long long int toss = 0; toss < number_of_tosses / world_size; toss++) {
        double x = (double)rand() / RAND_MAX * 2 - 1; // N�mero aleatorio entre -1 y 1
        double y = (double)rand() / RAND_MAX * 2 - 1; // N�mero aleatorio entre -1 y 1
        double distance_squared = x * x + y * y;
        if (distance_squared <= 1) number_in_circle++;
    }

    long long int global_sum;
    MPI_Reduce(&number_in_circle, &global_sum, 1, MPI_LONG_LONG_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (world_rank == 0) {
        double pi_estimate = 4 * (double)global_sum / (double)number_of_tosses;
        std::cout << "Estimacion de Pi: " << pi_estimate << std::endl;
    }

    MPI_Finalize();
    return 0;
}