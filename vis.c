#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(){
	int flag;
	read(STDIN_FILENO,&flag, sizeof(flag));
	Nodo * inicial;
	int cantidad = recibirDatos(inicial);
	Resultado * resultado = malloc(sizeof(Resultado));
	write(STDOUT_FILENO,resultado,sizeof(Resultado));
	return 0;
}