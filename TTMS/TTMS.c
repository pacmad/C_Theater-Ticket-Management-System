#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <io.h>
#include "./View/Main_Menu.h"

//系统运行入口函数main 
int main(void) {
	while(1){
			setvbuf(stdout, NULL, _IONBF, 0);
			Main_Menu();
	}
	return EXIT_SUCCESS;
}
