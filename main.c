#include "cabeceras.h"

void recibirArgumentos(int argc, char *argv[], int *n, int *flag){
	int flags, opt;
	char *aux3;
	char *nombre = (char*)malloc(sizeof(char)*60);
	char *rut= (char*)malloc(sizeof(char)*20);
	int edad;
	aux3 = malloc(10*sizeof(char));
	if(argc <8){//si se ingresa un numero de argumentos menor a 3, se finaliza la ejecucion del programa
		//Debe ser 3, porque el nombre del programa se considera como un argumento, siendo -h y el valor que acompañe a -h los dos argumentos faltantes. No se considera -m (que seria el cuarto argumento) porque es un flag que puede ser especificado por el usuario o no
		printf("Se ingreso un numero incorrecto de argumentos\n");
		fprintf(stderr, "Uso correcto: %s [-h numero entero] [-m]\n",
				   argv[0]);
		   exit(EXIT_FAILURE);
		}
	int N = -1;
	flags = 0;
	while((opt = getopt(argc, argv, "in:r:e:")) != -1) { //Los argumentos deban ir acompañados por algun valor, se deben acompañar de un ":" como se puede ver en h:, en el caso de -m esto no es necesario porque no se acompaña de ningun valor
	   switch(opt) {
	   case 'i'://se busca el flag -m, en caso de ser encontrado se setea el valor flags = 1, no se considera lo que se ingrese despues del flag -m
		   flags = 1;
		   break;
	   case 'n': //se busca el flag -n
	    	nombre= optarg;
		   break;
		case 'r':
			rut=optarg;
			
			break;
		case 'e':
			sscanf(optarg, "%i", &edad);
			break;
		   /*
	   default: /* '?' */
		   /*
		   fprintf(stderr, "Uso correcto: %s [-h numero entero] [-m]\n",
				   argv[0]);
		   exit(EXIT_FAILURE);*/
	   }
	}

	if(flags==1){//si se encontro un flag -m, se setea la variable global flag = 1, respecto al scope del proceso principal
		(*flag) = 1;
		//detecta el flag entonces se muestran los datos
		printf("%s\n", nombre);
		printf("%s\n", rut);
		printf("%i\n", edad);
		}
	 //se iguala la variable n a N, para poder acceder al valor en el main
	


}




int main(int argc, char *argv[]){
	leerArchivo("datos.csv");

	int n = 0, flag = 0;
	recibirArgumentos(argc, argv, &n, &flag);
	if(flag==1){
		printf("Se utilizo flag -m\n");
		}
	printf("El argumento de flag -h es: %d\n", n);


	return 0;
}