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

pid_t parentprocess;

void exit_shell(){
    exit_flag=1;
    kill(parentprocess,SIGKILL);

}


void clr() {
  printf("\033[H\033[J");
}


void pause_shell() {
    printf("shell paused press enter to continue");
    getchar();
}

void directory() {
    system("ls");
}

void echo(char **tokens, int size){
    for(int i=1;i<size;i++){
        printf("%s ",tokens[i]);
    }
    printf("\n");
    exit(0);
}

void process_tokens(char **tokens, int size) {

        if(strcmp(tokens[0],"cd")==0){
            printf("cd");
            
        } else if (strcmp(tokens[0],"clr")==0) {
            clr();
            
        } else if (strcmp(tokens[0],"dir")==0) {
            directory();
            
        }else if (strcmp(tokens[0],"environ")==0) {
            printf("env");
            
        }else if (strcmp(tokens[0],"echo")==0) {
            char **d ;
            strcpy(d,tokens);
            echo(tokens,size);
            
        }else if (strcmp(tokens[0],"help")==0) {
             printf("help");
            
        }else if (strcmp(tokens[0],"pause")==0) {
            pause_shell();
            
        }else if (strcmp(tokens[0],"quit")==0) {
            exit_shell();
            
        }
        
    
    


    
}

void tokenise_input(char *str){
    char* token;
    char delim[1] = " ";
    char ** tokenisedStr;
     token = strtok(str, delim);
    
    int i=0;
     
     while(token!=NULL){
       
         tokenisedStr[i] = token;
         i++;
         token = strtok(NULL,delim);
         
     }
    //i was incremented an extra time when for loop exited
    process_tokens(tokenisedStr,i--);
}

int readInput(){
    char * buff;
    buff = readline("\n");
    if(strlen(buff)!=0){
      if(fork()==0){
                tokenise_input(buff);
               exit(0);
           }
          
           free(buff);
        return 1;
    } else {
     
        return 0;
    }
   
}

int main(int argc, const char * argv[]) {
    parentprocess = getpid();
    while(1){
        waitpid(-1, NULL, WNOHANG);

        readInput();
        

    }
//
    printf("sadfasdf");
    return 0;
}
