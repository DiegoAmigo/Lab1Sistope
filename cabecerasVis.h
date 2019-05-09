#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
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

struct Resultado{
	float mediaReal;
	float mediaImaginaria;
	float potencia;
	float ruidoTotal;
	int pid;
};

typedef struct Resultado Resultado;

void agregar(Nodo * nuevoNodo, Nodo * nodoInicial);
void freeNodos(Nodo * nodoInicial);
int recibirDatos(Nodo * inicial);
float mediaReal(Nodo *nodoInicio, int anchoDisco);
float mediaImaginaria(Nodo *nodoInicio, int anchoDisco);
float potencia(Nodo *nodoInicio);
float ruidoTotal(Nodo *nodoInicio);