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
		free(aux->visibilidad);
		free(aux);
	}
}

int recibirDatos(Nodo * inicial){
	inicial = NULL;
	Nodo * aux;	
	char buffer[100];
	char * token;
	char * pend;
	int cantidad = 0;
	while(1) {
		read(STDIN_FILENO, buffer, sizeof(buffer));
		if(strcmp(buffer,"FIN") == 0){
			break;
		}
		cantidad++;
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
	return cantidad;
}

float mediaReal(Nodo *nodoInicio, int anchoDisco){
	float resultado=0;
	Nodo *aux=NULL;
	aux=nodoInicio;
	if (nodoInicio->siguiente==NULL){
		resultado = (nodoInicio->visibilidad->real);
	}
	else{
		while (aux->siguiente!=NULL){
			resultado = resultado + aux->visibilidad->real;
			aux=aux->siguiente;
		}
		resultado=resultado/anchoDisco;
	}
	return resultado;
}

float mediaImaginaria(Nodo *nodoInicio, int anchoDisco){
	float resultado=0;
	Nodo *aux=NULL;
	aux=nodoInicio;
	if (nodoInicio->siguiente==NULL){
		resultado = (nodoInicio->visibilidad->imaginaria);
	}
	else{
		while (aux->siguiente!=NULL){
			resultado = resultado + aux->visibilidad->imaginaria;
			aux=aux->siguiente;
		}
		resultado=resultado/anchoDisco;
	}
	return resultado;

}

float potencia(Nodo *nodoInicio){
	float resultado=0;
	Nodo *aux=NULL;
	aux=nodoInicio;
	if(nodoInicio->siguiente==NULL){
		resultado= sqrtf(powf(nodoInicio->visibilidad->real, 2) + powf(nodoInicio->visibilidad->imaginaria, 2));
	}
	else{
		while(aux->siguiente!=NULL){
			resultado= resultado + sqrtf(powf(nodoInicio->visibilidad->real, 2) + powf(nodoInicio->visibilidad->imaginaria, 2));
			aux=aux->siguiente;
		}
	}
	return resultado;
}

float ruidoTotal(Nodo *nodoInicio){
	float resultado=0;
	Nodo *aux=NULL;
	aux=nodoInicio;
	if(nodoInicio->siguiente==NULL){
		resultado=nodoInicio->visibilidad->ruido;
	}
	else{
		while(aux->siguiente!=NULL){
			resultado=resultado + aux->visibilidad->ruido;
			aux=aux->siguiente;
		}
	}
	return resultado;
}