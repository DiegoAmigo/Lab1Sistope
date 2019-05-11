#include "cabecerasMain.h"
#define WRITE 1                          
#define READ 0                            

//Función por la cual se inicia el proceso de ejecución y principalmente se leen los argumentos de entrada.
void recibirArgumentos(int argc, char *argv[], int *n, int *flag){
	int flags, opt;
	Nodo *inicio=NULL;
	//Se le asigna memoria a path de archivos
	char *nombreArchivo= calloc(100,sizeof(char));
	char *nombreSalida=calloc(100,sizeof(char));
	int cantDiscos;
	int anchoDiscos;
	if(argc <8){//si se ingresa un numero de argumentos menor a 8, se finaliza la ejecucion del programa
		printf("Se ingreso un numero incorrecto de argumentos\n");
		   exit(EXIT_FAILURE);
		}
	flags = 0;
	//Haciendo uso de getopt se guardan los datos de entrada
	while((opt = getopt(argc, argv, "i:o:n:d:b")) != -1) { //Los argumentos deban ir acompañados por algun valor, se deben acompañar de un ":" como se puede ver en h:, en el caso de -m esto no es necesario porque no se acompaña de ningun valor
	   switch(opt) {
	   case 'i':
		   strcpy(nombreArchivo,optarg);
		   break;
	   case 'o': 
	    	strcpy(nombreSalida,optarg);
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
	//Se llama a función leer archivo
	inicio=leerArchivo(nombreArchivo);
	//Se llama a función delegar, la cual realiza el proceso principal de la aplicación.
	delegar(inicio, cantDiscos, anchoDiscos, flags, nombreSalida);
	//Se hace free de los path de archivos
	free(nombreArchivo);
	free(nombreSalida);
}

//Función para hacer free de la lista completa
void freeNodos(Nodo * nodoInicial){
	Nodo * actual = nodoInicial;
	Nodo * aux;
	while(actual != NULL){
		aux = actual;
		actual = actual->siguiente;
		free(aux->visibilidad);
		free(aux);
	}
}

//Función que lee el archivo con los datos del espacio de Fourier
Nodo * leerArchivo(char * direccion){
	Nodo * inicial= NULL;
	Nodo * aux;	
	FILE* fp;
	char buffer[255];
	fp = fopen(direccion, "r");
	char * pend;
	//Se lee linea a linea
	while(fgets(buffer, 255, (FILE*) fp)) {
		//se asigna memoria al nuevo nodo
		aux = (Nodo *)malloc(sizeof(Nodo));
		aux->siguiente = NULL;
		//se asigna memoria para la linea (visibilidad) que contendrá el nodo
		aux->visibilidad = (char *)calloc(256,sizeof(char));
		strcpy(aux->visibilidad, buffer);
		//el nuevo nodo se agrega a la lista (se agrega como si fuera una pila para optimizar)
    	if(inicial == NULL){
    		inicial = aux;
    	}
    	else{
    		aux->siguiente = inicial;
    		inicial = aux;
    	}
	}
	fclose(fp);
	return inicial;
}

//Función para calcular el hijo al cual será envíado una visibilidad, se retorna el indice del pipe correspondiente a dicho hijo
int direccionarVisibilidad(char * visibilidad, int ancho, int ndiscos){
	char * token;
	strcpy(token,visibilidad);
	char * pend;
	token = strtok(token,",");
	//Se obtiene la coordenada U de la visibilidad
	float coordenadaU = strtof(token,&pend);
	token = strtok(NULL,",");
	//Se obtiene la coordenada V de la visibilidad
	float coordenadaV = strtof(token,NULL);
	//Se aplica la formula de la distancia, raíz de la suma de potencias de ambas coordenadas
	float distancia = sqrtf(powf(coordenadaU,2)+powf(coordenadaV,2));
	//EL indice final, se calcula dividiendo la distancia calculada por el ancho de disco.
	int indice = (int)distancia/ancho;
	//Los indices que superan el valor del último intervalo de distancias son asignados al último disco
	if(indice>=ndiscos-1){
		return ndiscos-1;
	}
	else{
		return indice;
	}
}

void delegar(Nodo * nodoInicial, int ndiscos, int ancho, int flag, char * nombreArchivo){
	if(ndiscos == 0){
		return;
	}
	//Se crea un arreglo de descriptores, cuya cantidad de elementos depende del numero de discos
	Descriptores ** descriptores = (Descriptores **)calloc(ndiscos,sizeof(Descriptores *));
	int i;
	Resultado ** resultados;
	//se asigna memoria para los descriptores de cada hijo y se les hace pipe
	for(i = 0; i<ndiscos; i++){
		descriptores[i] = (Descriptores *)malloc(sizeof(Descriptores));
		descriptores[i]->escritura = (int *)calloc(2,sizeof(int));
		descriptores[i]->lectura = (int *)calloc(2,sizeof(int));
		pipe(descriptores[i]->escritura);
		pipe(descriptores[i]->lectura);
	}
	//se crean los hijos, se establecen los canales de comunicación y se les hace exec
	for(i = 0; i<ndiscos; i++){
		descriptores[i]->pid = fork();
		if(descriptores[i]->pid == 0){
			//Se establecen los canales de escritura para que el padre envíe datos a sus hijos
			close(descriptores[i]->escritura[WRITE]);//Por este canal escribe el padre, por lo que se cierra para el hijo                
        	dup2(descriptores[i]->escritura[READ], STDIN_FILENO);//Se duplica el canal de lectura del hijo y se cambia por stdin_fileno     
        	//Se establecen los canales de lectura para que el padre obtenga datos de sus hijos                   
        	close(descriptores[i]->lectura[READ]);//Este canal de lectura es del padre, por lo que se cierra para el hijo
        	dup2(descriptores[i]->lectura[WRITE], STDOUT_FILENO);//Se duplica el canal de escritura del hijo y se cambia por stdout_fileno
			execl("./vis", "", "",(char*) 0);//Se hace execl con el hijo i
			exit(0);
		}
		//el padre cierra los descriptores que no son utilizados
		close(descriptores[i]->escritura[READ]);
		close(descriptores[i]->lectura[WRITE]);
	}
	//Se hace limpieza del stdout debido a que genera conflictos con los pipes.
	fputs("\033c", stdout);
	Nodo * actual = nodoInicial;
	int indiceDisco;
	actual = nodoInicial;
	//Se recorre la lista para hacer el envío de datos
	while(actual != NULL){
		//Se calcula el indice usando la distancia
		indiceDisco = direccionarVisibilidad(actual->visibilidad, ancho, ndiscos);
		//Se envía la visibilidad actual
		write(descriptores[indiceDisco]->escritura[WRITE], actual->visibilidad, 256);
		actual = actual->siguiente;
		//Se limpia el buffer stdout para evitar conflictos
		fputs("\033c", stdout);
	}
	resultados = (Resultado **)calloc(ndiscos,sizeof(Resultado *));
	//Una vez enviadas todas las visibilidades, se les envía un aviso de FIN a todos los hijos para que retornen los resultados
	float f;
	for(i = 0; i<ndiscos;i++){
		//Se le envía FIN al hijo i, para que deje de leer datos y retorne.
		write(descriptores[i]->escritura[WRITE],"FIN",100);
		//Se le asigna memoria a la struct Resultados, para guardar los datos provenientes del hijo
		resultados[i] = (Resultado *)malloc(sizeof(Resultado));

		//Se leen uno por uno los resultados de cada calculo
		read(descriptores[i]->lectura[READ],&f,sizeof(f));
		
		resultados[i]->mediaReal = f;
		read(descriptores[i]->lectura[READ],&f,sizeof(f));
		
		resultados[i]->mediaImaginaria = f;
		read(descriptores[i]->lectura[READ],&f,sizeof(f));
		
		resultados[i]->potencia = f;
		read(descriptores[i]->lectura[READ],&f,sizeof(f));
		
		resultados[i]->ruidoTotal = f;
		read(descriptores[i]->lectura[READ],&f,sizeof(f));
		resultados[i]->visibilidades = (int)f;
		resultados[i]-> pid = descriptores[i]->pid;
		//Se imprimen la cantidad de visibilidades procesadas por cada hijo si es que se usó la bandera
		if(flag == 1){
			printf("Soy el hijo de pid %d, procese %d visibilidades\n",resultados[i]->pid,resultados[i]->visibilidades);
		}
		wait(NULL);
	}
	//Se cierran y se hace free de los descriptores
	for(i=0;i<ndiscos;i++){
		close(descriptores[i]->lectura[READ]);
		close(descriptores[i]->escritura[WRITE]); 
		free(descriptores[i]->lectura);
		free(descriptores[i]->escritura);
		free(descriptores[i]);
	}
	free(descriptores);
	//Se escribe el archivo de salida
	salidaArchivo(nombreArchivo, resultados, ndiscos);
	//Se hace free de las struct resultados
	for(i = 0; i<ndiscos;i++){
		free(resultados[i]);
	}
	free(resultados);
}

//Función para escribir el archivo de salida.
void salidaArchivo(char *nombreArchivo, Resultado **resultado, int cantDiscos){
	FILE *salida=fopen(nombreArchivo, "w");
	for(int i=0;i<cantDiscos;i++){
		fprintf(salida, "%s %i\n%s %f\n", "Disco", i, "Media Real: ", resultado[i]->mediaReal);
		fprintf(salida, "%s %f\n", "Media Imaginaria: ", resultado[i]->mediaImaginaria);
		fprintf(salida, "%s %f\n", "Potencia: ", resultado[i]->potencia);
		fprintf(salida, "%s %f\n", "Ruido Total: ", resultado[i]->ruidoTotal);
	}
	fclose(salida);
}