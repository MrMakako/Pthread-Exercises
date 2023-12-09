#include <iostream>
#include <pthread.h>

#define MAX_THREADS 4

using namespace std; 

double a = 0.0, b = 3.0;
int n = 1024;
double h;
double total_int = 0.0;

// Función a integrar
double f(double x) {
    return x * x;
}

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* Trap(void* rank) {
    long my_rank = (long)rank;
    int local_n = n / MAX_THREADS;
    double local_a = a + my_rank * local_n * h;
    double local_b = local_a + local_n * h;
    double local_int = (f(local_a) + f(local_b)) / 2.0;

    for (int i = 1; i <= local_n - 1; i++) {
        double x = local_a + i * h;
        local_int += f(x);
    }

    local_int = local_int * h;

    pthread_mutex_lock(&mutex);
    total_int += local_int;
    pthread_mutex_unlock(&mutex);

    return NULL;
}

int main(int argc, char* argv[]) {

    pthread_t threads[MAX_THREADS];
    long thread;
    h = (b - a) / n;

    for (thread = 0; thread < MAX_THREADS; thread++) {
        pthread_create(&threads[thread], NULL, Trap, (void*)thread);
    }

    // Une los hilos al proceso principal.
    for (thread = 0; thread < MAX_THREADS; thread++) {
        pthread_join(threads[thread], NULL);
    }

    cout << "With n = " << n << " trapezoids, our estimate\n";
    cout << "of the integral from " << a << " to " << b << " = " << total_int << endl;

    return 0;
}
/*
Ventaja Mutex:
-Los mutex garantizan que solo un hilo a la vez pueda acceder a una sección 
crítica del código, evitando así condiciones de carrera y corrupción de datos.
-Mutex duerme los hilos en lugar de dejarlos en espera. Esto evita que hayan hilos gastando recursos sin estar realizando ninguna tarea.

Desventaja Mutex:
-Su implementación es más complicada y puede llevar a problemas si no se maneja con cuidado.

*/