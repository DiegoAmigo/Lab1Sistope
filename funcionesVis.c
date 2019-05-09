#include "cabecerasVis.h"

void agregar(Nodo * nuevoNodo, Nodo * nodoInicial){
	if(nodoInicial == NULL){
		nodoInicial = nuevoNodo;
	}
	else{
		Nodo * actual = nodoInicial;
		while(actual->siguiente != NULL){
			actual = actual->siguiente;
		}
		actual->siguiente = nuevoNodo;
	}
}

void freeNodos(Nodo * nodoInicial){
	Nodo * actual = nodoInicial;
	Nodo * aux;
	while(actual->siguiente != NULL){
		aux = actual;
		actual = actual->siguiente;
		free(aux);
	}
}

Nodo * leerArchivo(char * direccion){
	Nodo * inicial= NULL;
	Nodo * aux;	
	FILE* fp;
	char buffer[255];
	char * token;
	fp = fopen(direccion, "r");
	char * pend;
	while(fgets(buffer, 255, (FILE*) fp)) {
		aux = malloc(sizeof(Nodo));
		aux->siguiente = NULL;
		aux->visibilidad = malloc(sizeof(Visibilidad));
    	token = strtok(buffer,",");
    	aux->visibilidad->coordenadaU = strtof(token,&pend);
    	token = strtok(NULL,",");
    	aux->visibilidad->coordenadaV = strtof(token,NULL);
    	token = strtok(NULL,",");
    	aux->visibilidad->real = strtof(token,NULL);
    	token = strtok(NULL,",");
    	aux->visibilidad->imaginaria = strtof(token,NULL);
    	token = strtok(NULL,",");
    	aux->visibilidad->ruido = strtof(token,NULL);
    	agregar(aux, inicial);
	}
	fclose(fp);
}