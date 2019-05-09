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

struct Resultado{
	float mediaReal;
	float mediaImaginaria;
	float potencia;
	float ruidoTotal;
};

typedef struct Resultado Resultado;