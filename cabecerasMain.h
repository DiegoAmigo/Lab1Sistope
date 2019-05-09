#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

struct Nodo{
	struct Nodo * siguiente;
	char * linea;
};

typedef struct Nodo Nodo;

struct Resultado{
	float mediaReal;
	float mediaImaginaria;
	float potencia;
	float ruidoTotal;
};

typedef struct Resultado Resultado;

void agregar(Nodo * nuevoNodo, Nodo * nodoInicial);
void freeNodos(Nodo * nodoInicial);
Nodo * leerArchivo(char * direccion);
Resultado ** delegar(Nodo * nodoInicial, int ndiscos, int ancho, int flag);