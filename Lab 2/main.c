//
//  main.c
//  shell
//
//  Created by Pawan on 2020-02-07.
//  Copyright © 2020 uoit. All rights reserved.
//

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<readline/readline.h>
#include<readline/history.h>

int exit_flag = 0;

void exit(){
    exit_flag=1;
}


void clr() {
  printf("\033[H\033[J");
}


void pause_shell() {
    printf("shell paused press enter to continue");
    getchar();
}

void processToken(char *t){
    
}

void readInput(){
  
     char * buff = readline("\n");
     char command[256];
    char* token;
    strcpy(command,buff);
       char delim[1] = " ";
     token = strtok(command, delim);
     
     while(token!=NULL){
         printf("%s\n",token );
         
         token = strtok(NULL,delim);
         
     }
    
}

int main(int argc, const char * argv[]) {
    while(1 & !exit_flag){
      
        readInput();
        
        
    }
    return 0;
}
