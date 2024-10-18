#include <thread>
#include <mutex>
#include <iostream>
using namespace std;
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

mutex myMutex;
// Declarar el podio para usarlo en las funciones
int *PODIO;

// cuando un auto termina la carrera sube al podio
void subirPodio(int numero)
{
    int i = 0;
    while ((PODIO[i] != 0))//buscar indice en que esta desocupado
    {
        i++;
    }
    PODIO[i] = numero;//ingresar el numero del auto que termino
    // printf("auto %d, lugar %d \n",PODIO[i], i+1);
}

// cada hebra comienza con esta funcion en la que avanzan y se detienen aleatoreamente
void competir(int distancia, int numero, int seed)
{
    srand(seed); // semilla aleatoria
    int recorrido = 0;
    while (distancia > 0)
    {
        int avance = rand() % 10 + 1;            // generar avance
        int espera = (rand() % 400000) + 100000; // tiempo de espera
        distancia -= avance;//distancia restante
        recorrido += avance;//Distancia recorrida
        usleep(espera);
        printf("El auto %d avanza %d metros, distancia recorrida: %d\n", numero, avance,recorrido);
        // printf("El auto %d espera %f segundos\n", numero, espera);
    }
    myMutex.lock();//bloqueo para sincronizar el uso del podio
    printf("°°°El auto %d termino la carrera°°°\n", numero);
    subirPodio(numero); // al terminar la carrera el auto sube al podio
    myMutex.unlock();
}

int main(int argc, char *argv[])
{

    // Validar el argc
    if (argc != 3)
    {
        cout << "uso: ./carrera <Distancia> <Autos>\n";
        return -1;
    }
    // validar distancia y catidad de autos
    if (atoi(argv[1]) <= 0 || atoi(argv[2]) <= 0)
    {
        cout << "La distancia y los autos deben ser > 0\n";
        return -1;
    }

    // inicializar podio
    PODIO = new int[atoi(argv[2])]();

    cout << "Iniciando carrera\n";
    thread *autos[atoi(argv[2])];
    int distancia = atoi(argv[1]);
    cout << "Carrera de: " << distancia << " metros\n";

    // crear las hebras segun la cantidad de autos
    for (int i = 0; i < atoi(argv[2]); i++)
    {
        unsigned int seed = time(NULL);
        autos[i] = new thread(competir, distancia, i + 1, seed + i);
    }

    //Esperar a que la hebras (autos) terminen.
    for (int i = 0; i < atoi(argv[2]); i++)
    {
        autos[i]->join();
        delete autos[i];
    }

    cout << "Termino la carrera\n______________________\n";
    cout << "Podio:\n";

    // Mostrar estado final del podio
    int i = 0;

    do
    {//leer el coontenido del podio 
        printf("Puesto %d auto: %d\n", i + 1, PODIO[i]);
        i++;
    } while (i < atoi(argv[2]));

    return 0;
}