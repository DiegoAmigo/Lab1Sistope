#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(){
	int flag;
	read(STDIN_FILENO,&flag, sizeof(flag));
	
	return 0;
}