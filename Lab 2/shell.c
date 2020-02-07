#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shell.h"

#define clear() printf("\033[H\033[J")
// #define gotoxy(x, y) printf("\033[%d;%dH", y, x)

int main(){
	// int a = 5, b = 6;
	// printf("HELLO WORLD\n");
	// printf("++a: %d,a: %d\n", ++a,a);
	// printf("a++: %d,a: %d\n", a++,a);
	// printf("--a: %d,a: %d\n", --a,a);
	// printf("a--: %d,a: %d\n", a--,a);
	getchar();
    printf("Printing current dir\n");
	char cwd[100];
	if(getcwd(cwd, sizeof(cwd)) != NULL){
		printf("%s\n", cwd);
	}
	getchar();
	clear();
	getchar();
	// gotoxy(5, 6);
    showsomething();
	getchar();
    // print_env_vars();
    // getchar();

    char str[] = "A clever fox jumped over a lazy dog";
    // printf("-------%s", &str);
    char *token = strtok(str, " ");
    
    do{
		// token = strtok(str, " ");
        printf("%s\n", token);
        token = strtok(NULL, " ");
    }while(token != NULL);

	return 0;
}

void print_env_vars(){
    // const char* s = getenv("USER");
    const char* s = getenv("PATH");
    printf("%s\n", (s != NULL) ? s : "");
}

