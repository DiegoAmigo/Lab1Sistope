#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "cabecerasVis.h"
int main(){
	int flag;
	read(STDIN_FILENO,&flag, sizeof(flag));
	Nodo * inicial;
	int ancho = recibirDatos(inicial);
	Resultado * resultado = malloc(sizeof(Resultado));
	resultado->mediaReal = mediaReal(inicial,ancho);
	resultado->mediaImaginaria = mediaImaginaria(inicial, ancho);
	resultado->potencia = potencia(inicial);
	resultado->ruidoTotal = ruidoTotal(inicial);
	resultado->pid = getpid();
	write(STDOUT_FILENO,resultado,sizeof(Resultado));
	if(flag){
		printf("Soy el hijo de pid %d, procese %d visibilidades",getpid(),ancho);
	}
	return 0;
}