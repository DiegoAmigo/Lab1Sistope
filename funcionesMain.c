#include "cabecerasMain.h"
#define WRITE 1                          
#define READ 0                            


void recibirArgumentos(int argc, char *argv[], int *n, int *flag){
	int flags, opt;
	Nodo *inicio=NULL;
	Resultado **resultados=NULL;
	char *nombreArchivo = (char*)malloc(sizeof(char)*60);
	char *nombreSalida= (char*)calloc(60, sizeof(char));
	int cantDiscos;
	int anchoDiscos;
	if(argc <8){//si se ingresa un numero de argumentos menor a 3, se finaliza la ejecucion del programa
		//Debe ser 3, porque el nombre del programa se considera como un argumento, siendo -h y el valor que acompañe a -h los dos argumentos faltantes. No se considera -m (que seria el cuarto argumento) porque es un flag que puede ser especificado por el usuario o no
		printf("Se ingreso un numero incorrecto de argumentos\n");
		fprintf(stderr, "Uso correcto: %s [-h numero entero] [-m]\n",
				   argv[0]);
		   exit(EXIT_FAILURE);
		}
	flags = 0;
	while((opt = getopt(argc, argv, "i:o:n:d:b")) != -1) { //Los argumentos deban ir acompañados por algun valor, se deben acompañar de un ":" como se puede ver en h:, en el caso de -m esto no es necesario porque no se acompaña de ningun valor
	   switch(opt) {
	   case 'i'://se busca el flag -m, en caso de ser encontrado se setea el valor flags = 1, no se considera lo que se ingrese despues del flag -m
		   nombreArchivo=optarg;
		   break;
	   case 'o': //se busca el flag -n
	    	nombreSalida= optarg;
		   break;
		case 'n':
			sscanf(optarg, "%i", &cantDiscos);
			break;
		case 'd':
			sscanf(optarg, "%i", &anchoDiscos);
			break;
		case 'b':
			flags=1;
			break;
	   }
	}

	if(flags==1){//si se encontro un flag -m, se setea la variable global flag = 1, respecto al scope del proceso principal
		(*flag) = 1;
		//detecta el flag entonces se muestran los datos
		inicio=leerArchivo(nombreArchivo);
		resultados=delegar(inicio, cantDiscos, anchoDiscos, *flag);
		salidaArchivo(nombreSalida, resultados, cantDiscos);
		printf("%s\n", nombreArchivo);
		printf("%s\n", nombreSalida);
		printf("%i\n", cantDiscos);
		printf("%i\n", anchoDiscos);
	}
	//freeNodos(inicio);
	int l;
	for(l = 0; l<cantDiscos; l++){
		printf("%f, %f, %f, %f\n", resultados[l]->mediaReal, resultados[l]->mediaImaginaria, resultados[l]->potencia, resultados[l]->ruidoTotal);
		printf("Soy el hijo de pid %d, procese %d visibilidades\n",resultados[l]->pid,resultados[l]->visibilidades);
		free(resultados[l]);
	}
	free(resultados);
}


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
		free(aux->linea);
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
		aux->linea = (char *)calloc(100,sizeof(char));
    	strcpy(aux->linea,buffer);
    	agregar(aux, inicial);
	}
	fclose(fp);
	return inicial;
}

Resultado ** delegar(Nodo * nodoInicial, int ndiscos, int ancho, int flag){
	int ** descriptores = (int **)calloc(ndiscos*2, sizeof(int *));
	int i;
	int * pids = (int *)calloc(ndiscos, sizeof(int));
	//se asigna memoria para los descriptores de cada hijo y se les hace pipe
	for(i = 0; i<ndiscos*2; i++){
		descriptores[i] = (int *)calloc(2,sizeof(int));
		pipe(descriptores[i]);
	}
	//se crean los hijos, se establecen los canales de comunicación y se les hace exec
	int j;
	for(i = 0; i<ndiscos; i++){
		pids[i] = fork();
		j = i*2;
		if(pids[i] == 0){
			//descriptor[j] será para que el padre le envíe datos al hijo i
			close(descriptores[j][WRITE]);//Se cierra el canal de escritura del descriptor[j]
			dup2(descriptores[j][READ],STDIN_FILENO);//El canal de lectura del descriptor[j] se duplica y pasa a ser el standar input
			close(descriptores[j][READ]);//Se cierra el canal de lectura que no se usará del descriptor 
			//descriptor[j+1] será para que el hijo envíe datos al padre
			close(descriptores[j+1][READ]);//Se cierra el canal de lectura del descriptor[j+1]
			dup2(descriptores[j+1][WRITE],STDOUT_FILENO);//El canal de escritura del decriptor[j+1] se duplicar y pasa a ser el standar output
			close(descriptores[j+1][WRITE]);//Se cierra el canal de escritura que no se usará del descriptor 
			execl("./vis", "", "",(char*) 0);//Se hace execl con el hijo i
		}
		//el padre cierra los descriptores que no son necesarios
		close(descriptores[j][READ]);
		close(descriptores[j+1][WRITE]);
	}
	//Ahora se envían a los datos a los hijos a través de los pipes
	//Con este ciclo se envían los datos a los primeros n - 1 hijos, ya que estos reciben un número fijo de visibilidades (ancho)
	Nodo * actual = nodoInicial;
	int k;
	for(i = 0; i<ndiscos-1;i++){
		j = i*2;
		//Primero se envía el flag para indicar al hijo i si es que debe hacer print de la cantidad de visibilidades que procesó
		write(descriptores[j][WRITE],&flag,sizeof(flag));
		//Si no quedan visibilidades por enviar, se termina el ciclo.
		if(actual == NULL){
			break;
		}
		//Se envían tantas visibilidades al hijo i como el valor del ancho
		for(j = 0;j<ancho && actual != NULL; j++){
			write(descriptores[j][WRITE],actual->linea,100);
			actual= actual->siguiente;
		}
	}
	//al ultimo hijo se le envían todas las visibilidades restantes
	j = i*2;
	//Se envía flag
	write(descriptores[j][WRITE],&flag,sizeof(flag));
	while(actual != NULL){
		write(descriptores[j][WRITE],actual->linea,100);
		actual= actual->siguiente;
	}
	//Una vez enviadas todas las visibilidades, se les envía un aviso de FIN a todos los hijos
	for(i = 0; i<ndiscos;i++){
		j = i*2;
		write(descriptores[j][WRITE],"FIN",5);
		close(descriptores[j][WRITE]); //Se cierra descriptor del padre para escritura ya que no se hará mas uso de el.
	}
	//Se leen los resultados de los hijos.
	
	Resultado ** resultados = calloc(ndiscos,sizeof(Resultado *));
	for(i = 0; i<ndiscos; i++){
		j = i*2;
		resultados[i] = malloc(sizeof(Resultado));
		read(descriptores[j][READ],resultados[i],sizeof(resultados[i]));
	}
	return resultados;
}

void salidaArchivo(char *nombreArchivo, Resultado **resultado, int cantDiscos){
	FILE *salida=fopen(nombreArchivo, "a");
	for(int i=0;i<cantDiscos;i++){
		fprintf(salida, "%s %i\n %s %f", "Disco", i, "Media Real: ", resultado[i]->mediaReal);
		fprintf(salida, "%s %f", "Media Imaginaria: ", resultado[i]->mediaImaginaria);
		fprintf(salida, "%s %f", "Potencia: ", resultado[i]->potencia);
		fprintf(salida, "%s %f", "Ruido Total: ", resultado[i]->ruidoTotal);
	}
	fclose(salida);
}