#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <io.h>
#include "./View/Main_Menu.h"

//ϵͳ������ں���main 
int main(void) {
	while(1){
			setvbuf(stdout, NULL, _IONBF, 0);
			Main_Menu();
	}
	return EXIT_SUCCESS;
}
