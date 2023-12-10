#include <mpi.h>
#include <iostream>
#include <cstdlib>
#include <random>
#include <iomanip>

using namespace std;

// Generador de números.
long int gen_numbers(long int range) {
    return rand() % range;
}

// Genera un array con números random.
long int * gen_array( long int size )
{
    long int * temp_arr = new long int[size];

    for ( int i = 0; i < size; i++ )
    {
        temp_arr[i] = gen_numbers(size);
    }
    
    return temp_arr;
}

// Sort function
void odd_even_sort(long int *arr, long int size)
{
    for(int step = 0; step < size; step++) // Sorting loop
    {
        if(step % 2 == 0)
        { // EVEN.
            for(int i = 1; i < size; i += 2)
            {
                if(arr[i - 1] > arr[i]) // iterador con iterador - 1
                {
                    swap(arr[i - 1], arr[i]);
                }
            }
        }
        else 
        { // ODD.
            for(int i = 1; i < size - 1; i += 2)
            {
                if(arr[i] > arr[i + 1]) // iterador con iterador + 1
                {
                    swap(arr[i], arr[i + 1]);
                }
            }
        }
    }
}


// Imprime arrays.
void mostrar_array( long int * arr, long int size )
{
    for( long int i = 0; i < size; i++ )
    {
        cout << arr[i] << " ";
    }
    cout << endl;
}


// OddEven control de phases.
void PHASE(long int SEND_RANK, long int RCV_RANK, long int *arr, int size, MPI_Comm COMM)
{
    // Obtiene el rank actual.
    int current_rank;
    MPI_Comm_rank(COMM, &current_rank);

    // Array para hacer la unión de ambos arrays.
    long int *temp_arr = new long int[size];

    // Array con los valores de ambos procedimientos.
    long int *aux_arr = new long int[size * 2];

    if (current_rank == SEND_RANK)
    {
        MPI_Send(arr, size, MPI_LONG, RCV_RANK, 0, COMM);
        // Espera a recibir el arreglo del otro rank.
        MPI_Recv(arr, size, MPI_LONG, RCV_RANK, 1, COMM, MPI_STATUS_IGNORE);
    }
    else
    {
        // Recibe del array del otro rank.
        MPI_Recv(temp_arr, size, MPI_LONG, SEND_RANK, 0, COMM, MPI_STATUS_IGNORE);

        // Unión de ambos arreglos.
        int i = 0, j = 0, k = 0;

        // Fusiona y ordena ambos arreglos en un solo arreglo temporal (aux).
       for (int k = 0; k < size * 2; ++k)
        {
            if (i < size && (j == size || arr[i] < temp_arr[j]))
            {
                aux_arr[k] = arr[i++];
            }
            else
            {
                aux_arr[k] = temp_arr[j++];
            }
        }

        // Agrega elementos sobrantes.
        while (i < size)
        {
            aux_arr[k++] = arr[i++];
        }

        while (j < size)
        {
            aux_arr[k++] = temp_arr[j++];
        }

        // Vuelve a dividir el arreglo ya ordenado para cada uno de los procesos.
        for (int i = 0; i < size; i++)
        {
            temp_arr[i] = aux_arr[i];
            arr[i] = aux_arr[size + i];
        }

        delete[] aux_arr;

        // Se retorna el arreglo ya ordenado.
        MPI_Send(temp_arr, size, MPI_LONG, SEND_RANK, 1, COMM);

        delete[] temp_arr;
    }
}


int main ( int argc, char *argv[] )
{

    // Verificar que le haya pasado un valor para la creación del array.
    if( argc != 2 )
    {
        cout << "Indique el tamaño del array!\n";
        return EXIT_FAILURE;
    }

    if( argc == 2 and atol(argv[1]) < 1 )
    {
        cout << "Tamaño inválido!\n";
        return EXIT_FAILURE;
    }

    int my_rank, comm_sz;

    MPI_Init(NULL, NULL); 
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    long int size = atol(argv[1]); // Array size.
    long int * arr = gen_array(size); // main array

    int local_size = size/comm_sz; // local list size
    long int * local_arr = new long int[local_size]; // local list

    // Mostramos el arreglo desde el rank 0.
    if( my_rank == 0 )
    {  
        cout << "\nArray antes del sort:\n";
        mostrar_array( arr, size ); 
        cout << "\n";
    }

    MPI_Scatter(arr, local_size, MPI_LONG, local_arr, local_size, MPI_LONG, 0, MPI_COMM_WORLD); 
    
    delete[] arr;

    // LOCAL SORT
    odd_even_sort( local_arr, local_size);

    // PHASES
    for( int proc_itr = 1; proc_itr <= comm_sz; proc_itr++)
    {
         if( ( my_rank + proc_itr ) % 2 == 0 ) // Odd
         {
            if( my_rank < comm_sz - 1 )
             {
                PHASE( my_rank, my_rank + 1, local_arr, local_size, MPI_COMM_WORLD);
            }  
         }
         else if( my_rank > 0 ) // Even
         {
            PHASE( my_rank - 1, my_rank, local_arr, local_size, MPI_COMM_WORLD );  
         }
    }

    long int * final_arr = new long int[size];

    // GATHERING
    MPI_Gather(local_arr, local_size, MPI_LONG, final_arr, local_size, MPI_LONG, 0, MPI_COMM_WORLD);
   
    if( my_rank == 0 )
    {
        cout << "Array después del sort:\n";
        mostrar_array( final_arr, size );
        cout << "\n"; 
    }

    delete[] final_arr;

    MPI_Finalize();
    
    return 0;
}