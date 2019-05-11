#include "cabecerasVis.h"

void procesar(){
	float resultados[5] = {0,0,0,0,0};
	char * buffer = (char *)calloc(256, sizeof(char));
	char * token;
	char * pend;
	float sumaReal = 0;
	float sumaImaginaria = 0;
	float sumaPotencia = 0;
	float sumaRuidoTotal = 0;
	int contador = 0;
	float parteReal;
	float parteImaginaria;
	float ruido;
	while(1){
		read(STDIN_FILENO, buffer, 256);
		fflush(STDIN_FILENO);
		if(strcmp(buffer,"FIN") == 0){
			close(STDIN_FILENO);
			if(contador != 0){
				resultados[0] = sumaReal/contador;
				resultados[1] = sumaImaginaria/contador;
				resultados[2] = sumaPotencia;
				resultados[3] = sumaRuidoTotal;
				resultados[4] = contador;
			}
			write(STDOUT_FILENO,&resultados[0],sizeof(resultados[0]));
			fflush(stdout);
			write(STDOUT_FILENO,&resultados[1],sizeof(resultados[1]));
			fflush(stdout);
			write(STDOUT_FILENO,&resultados[2],sizeof(resultados[2]));
			fflush(stdout);
			write(STDOUT_FILENO,&resultados[3],sizeof(resultados[3]));
			fflush(stdout);
			write(STDOUT_FILENO,&resultados[4],sizeof(resultados[4]));
			fflush(stdout);
			close(STDOUT_FILENO);
			break;
		}
		contador++;
		token = strtok(buffer,",");
    	token = strtok(NULL, ",");
    	token = strtok(NULL, ",");
    	parteReal = strtof(token,&pend);
    	token = strtok(NULL, ",");
    	parteImaginaria = strtof(token, &pend);
    	token = strtok(NULL, ",");
    	ruido = strtof(token, &pend);
    	sumaReal = sumaReal+parteReal;
    	sumaImaginaria = sumaImaginaria+parteImaginaria;
    	sumaPotencia = sumaPotencia + sqrtf(powf(parteReal,2)+powf(parteImaginaria,2));
    	sumaRuidoTotal = sumaRuidoTotal+ruido;
	}
	free(buffer);
}
