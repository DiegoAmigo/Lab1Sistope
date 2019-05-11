#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include  <sys/wait.h>     
#include <math.h>

//Struct para listar
struct Nodo{
	struct Nodo * siguiente;
	char * visibilidad;
};

typedef struct Nodo Nodo;

//Struct para guardar los resultados obtenidos desde los hijos
struct Resultado{
	float mediaReal;
	float mediaImaginaria;
	float potencia;
	float ruidoTotal;
	pid_t pid;
	int visibilidades;
};

typedef struct Resultado Resultado;

//Struct para guardar descriptores de cada hijo creado
struct Descriptores{
	int * escritura;//descriptor de escritura desde el padre a los hijo
	int * lectura;//descriptor de lectura que usa el padre para leer a sus hijos
	pid_t pid;
};

typedef struct Descriptores Descriptores;

void delegar(Nodo * nodoInicial, int ndiscos, int ancho, int flag, char * nombreArchivo);
void recibirArgumentos(int argc, char *argv[], int *n, int *flag);
void freeNodos(Nodo * nodoInicial);
Nodo * leerArchivo(char * direccion);
void salidaArchivo(char *nombreArchivo, Resultado **resultado, int cantDiscos);
int direccionarVisibilidad(char * visibilidad, int ancho, int ndiscos);