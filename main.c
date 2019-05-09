#include "cabecerasMain.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>


int main(int argc, char *argv[]){
	int n = 0, flag = 0;
	recibirArgumentos(argc, argv, &n, &flag);
	return 0;
}