#include <thread>
#include <mutex>
#include <iostream>
using namespace std;
#include <stdlib.h> 
#include <unistd.h> 

mutex myMutex;
//Podio para maximo 100 autos
int PODIO[100];

//cuando un auto termina la carrera sube al podio
void subirPodio(int numero)
{
    int i = 0;
    while ((PODIO[i] != 0))
    {
        i++;
    }
    PODIO[i] = numero;
    //printf("auto %d, lugar %d \n",PODIO[i], i+1);
}

// cada hebra comienza con esta funcion en la que avanzan y se detienen aleatoreamente
void competir(int distancia, int numero)
{
    while (distancia > 0)
    {
        int avance = rand() % 11;
        float espera = ((rand() % 4) + 1) / 10;
        distancia -= avance;
        printf("El auto %d avanza %d metros\n", numero, avance);
        sleep(espera);
    }
    printf("El auto %d termino la carrera\n", numero);
    myMutex.lock();
    subirPodio(numero);//al terminar la carrera el auto sube al podio
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

    cout << "Iniciando carrera\n";
    thread *autos[atoi(argv[2])]; // crea una hebra por cada auto
    int distancia = atoi(argv[1]);
    cout << "Carrera de :" << distancia << " metros\n";

    // crear las hebras
    for (int i = 0; i < atoi(argv[2]); i++)
    {
        autos[i] = new thread(competir, distancia, i + 1);
    }

    // Para esperar a que la hebras (hijas) terminen.
    for (int i = 0; i < atoi(argv[2]); i++)
    {
        autos[i]->join();
    }

    cout << "Termino la carrera\n";
    cout << "Podio:\n";
    
    //Mostrar estado final del podio
    int i=0;
    
    do{
        printf("%d puesto auto: %d\n",i+1,PODIO[i]);
        i++;
    }while (PODIO[i]!=0);
}