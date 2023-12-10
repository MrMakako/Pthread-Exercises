#include <iostream>
#include <mpi.h>
using namespace std;

int main(){
    int size, rank;

    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int n = 4;
    double matrix[n][n];
    double vector[n];
    double vector_resultado[n];
    int n_local=n/size;

        int matrixValues[] = {1, 2, 3, 4, 2, 4, 6, 8, 3, 6, 9, 12, 4, 8, 12, 16};

        // Rellena la matriz.
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                matrix[i][j] = matrixValues[i * n + j];
            }
        }

    double local_matrix[n][n_local];
    double vector_resultado_local[n];
    int recvcounts[n];
    for (int i = 0; i <size; i++) {
        recvcounts[i] = n;
    }

    // Inicializamos aquí el vector porque no estamos usando MPI_Bcast para compartir sus datos.

    int vectorValues[n] = {1, 2, 3, 4};
    for (int i = 0; i < n; i++){
        vector[i] = vectorValues[i];
    }

    // Reservando espacio para cada proceso.
    const int local_matrix_size = n * n / size;
    const int local_vector_resultado_size = n / size;
    // Repartición de la matrix a cada proceso.
   // MPI_Scatter(matrix, local_matrix_size, MPI_FLOAT, local_matrix, local_matrix_size, MPI_FLOAT, 0, MPI_COMM_WORLD);

    // Realizar la multiplicación local de la matriz por el vector.

    for (int i = 0; i< n; i++) {         // get all the rows now
        for (int j = 0; j <n_local; j++) { // only local_n columns
          local_matrix[i][j] = matrix[i][j + rank * n_local];
        }
    }
    for (int i = 0; i < n; i++) {
        vector_resultado_local[i] = 0.0;
        for (int j = 0; j < n_local; j++) {
            vector_resultado_local[i] += (local_matrix[i][j]  * vector[j + rank *n_local]);

        }
    }
    //Reduce_Scatter toma nuestro vector reusltado de cada proceso lo envia y lo suma en en el vector resultado 
    /*
        la suma se reliza tal que 
         A  +   B    
        |a|    |c|
        |b|    |d|
        |c|    |e|

        donde A y B...C son las multipplicaciones de bloque de la matriz por una columna de nuestro vector[n];
        donde A y B...C se asignan a un Procesos p1,p2....p3 igual al  n;
        recv counts  un array  de tamano [n] con {n,n,n,n} elementos representa la cantidad de operacion sobre cada elemnto del vecor resutlado de 
        cada proceso

    */
    MPI_Reduce_scatter(vector_resultado_local, vector_resultado, recvcounts, MPI_DOUBLE, MPI_SUM,
                     MPI_COMM_WORLD);
    if (rank == 0) {
              // Imprime la matriz y el vector.
        cout << "\n\nMatriz:" << endl;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                cout << matrix[i ][j] << "\t";
            }
            cout << endl;
        }

        cout << "\n\nVector:" << endl;
        for (int i = 0; i < n; i++) {
            cout << vector[i] << "\t";
        }
        cout << endl;
        cout << "\n\nVector resultante:\n";
        for (int i = 0; i < n; i++){
            cout << vector_resultado[i] << "\t";
        }
        cout << "\n\n";
    }
    MPI_Finalize();

    return 0;
}