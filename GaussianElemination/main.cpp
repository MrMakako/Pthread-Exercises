#include <iostream>
#include <omp.h>
using namespace std;
const int N = 3;
void printMatriz(float matrix[N][N + 1])
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N + 1; j++)
        {
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }
}

void gaussianElimination(float matrix[N][N + 1])
{
    for (int pivot = 0; pivot < N - 1; pivot++)
    { // itera a través de los pivotes
#pragma omp parallel for num_threads(N - 1)
        for (int i = pivot + 1; i < N; i++)
        {                                                             // itera a través de las filas del valor pivote, empezando en la fila pivote + 1
            float escalar = -matrix[i][pivot] / matrix[pivot][pivot]; // encuentra escalar para volver 0 los valores bajo el pivote.
            for (int j = pivot; j < N + 1; j++)
            {                                               // itera por cada columna de la fila
                matrix[i][j] += escalar * matrix[pivot][j]; // actualiza los valores de la fila actual sumando la multiplicación del escalar con la fila pivote.
            }
        }
    }
}

int main()
{
    float matrizA[N][N + 1] = {
        {2, -3, 0, 3},
        {4, -5, 1, 7},
        {2, -1, -3, 5},
    };

    cout << "Matriz Original" << endl;
    printMatriz(matrizA);
    double start = omp_get_wtime();
    gaussianElimination(matrizA);
    double end = omp_get_wtime();
    cout << "Matriz luego de eliminación de Gauss" << endl;
    printMatriz(matrizA);
    cout << "Tiempo =" << to_string(end - start) << endl;
    return 0;
}