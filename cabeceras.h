#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

struct Visibilidad{
	float coordenadaU;
	float coordenadaV;
	float real;
	float imaginaria;
	float ruido;
};

typedef struct Visibilidad Visibilidad;

struct Nodo{
	struct Nodo * siguiente;
	Visibilidad * visibilidad;
};

typedef struct Nodo Nodo;

void agregar(Nodo * nuevoNodo, Nodo * nodoInicial);
void freeNodos(Nodo * nodoInicial);
Nodo * leerArchivo(char * direccion);
