#include "cabecerasVis.h"

//En esta función se lleva a cabo el procesamiento de cada hijo
void procesar(){
	float resultados[5] = {0,0,0,0,0};
	//Se definen strings auxiliares para el procesamientos de las visibilidades enviadas por el padre
	char * buffer = (char *)calloc(256, sizeof(char));
	char * token;
	char * pend;
	//Se definen variables para calcular sumatorias de cada disco
	float sumaReal = 0;
	float sumaImaginaria = 0;
	float sumaPotencia = 0;
	float sumaRuidoTotal = 0;
	//contador de visibilidades procesadas
	int contador = 0;
	//variables auxiliares para leer datos de la visibilidad
	float parteReal;
	float parteImaginaria;
	float ruido;
	while(1){
		//Se lee un dato desde el stdin_fileno (descriptor definido desde el proceso padre)
		read(STDIN_FILENO, buffer, 256);
		fflush(STDIN_FILENO);
		//Si el string enviado es un 'FIN', se deja de leer y se hacen los calculos finales
		if(strcmp(buffer,"FIN") == 0){
			close(STDIN_FILENO);//se cierra descriptor de lecutra
			//Se aplican formulas requeridas y se guardan en un arreglo simple
			if(contador != 0){
				resultados[0] = sumaReal/contador;
				resultados[1] = sumaImaginaria/contador;
				resultados[2] = sumaPotencia;
				resultados[3] = sumaRuidoTotal;
				resultados[4] = contador;
			}
			//Se procede a enviar los resultados al padre, usando el stdout_fileno 
			write(STDOUT_FILENO,&resultados[0],sizeof(resultados[0]));
			write(STDOUT_FILENO,&resultados[1],sizeof(resultados[1]));
			write(STDOUT_FILENO,&resultados[2],sizeof(resultados[2]));
			write(STDOUT_FILENO,&resultados[3],sizeof(resultados[3]));
			write(STDOUT_FILENO,&resultados[4],sizeof(resultados[4]));
			close(STDOUT_FILENO);//se cierra descriptor de escritura
			break;
		}
		//a menos que el dato leido se un FIN, se aumenta el contador de visibilidades recibidas
		contador++;
		//se usa strtok para hacer split y strtof para convertir char * a float
		token = strtok(buffer,",");
    	token = strtok(NULL, ",");
    	token = strtok(NULL, ",");
    	parteReal = strtof(token,&pend);
    	token = strtok(NULL, ",");
    	parteImaginaria = strtof(token, &pend);
    	token = strtok(NULL, ",");
    	ruido = strtof(token, &pend);
    	//se hacen las sumas parciales según corresponde
    	sumaReal = sumaReal+parteReal;
    	sumaImaginaria = sumaImaginaria+parteImaginaria;
    	sumaPotencia = sumaPotencia + sqrtf(powf(parteReal,2)+powf(parteImaginaria,2));
    	sumaRuidoTotal = sumaRuidoTotal+ruido;
	}
	free(buffer);
}
