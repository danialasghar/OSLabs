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
#include <dirent.h>
#include<sys/wait.h>
#include<readline/readline.h>
#include<readline/history.h>

pid_t parentprocess;
extern char **environ;

void exit_shell(){
    //exit_flag=1;
    kill(parentprocess,SIGKILL);
}

void clr() {
  printf("\033[H\033[J");
}

void pause_shell() {
    printf("shell paused press enter to continue");
    getchar();
}

void directory(char **tokens) {
    DIR *directory;
    struct dirent *tempdir;
    directory = opendir(tokens[1]);
    if(directory){
        while((tempdir = readdir(directory)) != NULL)
        {
          printf("%s\n", tempdir -> d_name);
        }
        closedir(directory);
    }
}

void echo(char **tokens, int size){
    for(int i=1;i<size;i++){
        printf("%s ",tokens[i]);
    }
    printf("\n");
    exit(0);
    
}

void env() {
    char *ptr = *environ;

    for (int i = 1; i < ptr; i++) {
        printf("%s\n", ptr);
        ptr = *(environ+i);
    }
}

void help() {
    system("more README.md");
}

void change_directory(char **directory){
     printf(directory[1]);
    if(directory[1]==NULL){
        system("pwd");
    }
    else if (chdir(directory[1]) == -1){
        printf("%s: no such directory\n", directory[1]);
    }
    else{
        chdir(directory[1]);
        setenv("pwd",directory[1],1);
        system("pwd");
    }
}

void process_tokens(char **tokens, int size) {
    if(strcmp(tokens[size-1], "&")==0) {
      setpgid(0, 0);
      printf("Background process %d running %s.\n", getpid(), tokens[0]);
    }

    if(strcmp(tokens[0],"cd")==0){
        change_directory(tokens);
    } else if (strcmp(tokens[0],"clr")==0) {
        clr();
    } else if (strcmp(tokens[0],"dir")==0) {
        directory(tokens);
        
    }else if (strcmp(tokens[0],"environ")==0) {
        env();

    }else if (strcmp(tokens[0],"echo")==0) {
        char **d ;
        strcpy(d,tokens);
        echo(tokens,size);

    }else if (strcmp(tokens[0],"help")==0) {
            help();

    }else if (strcmp(tokens[0],"pause")==0) {
        pause_shell();

    }else if (strcmp(tokens[0],"quit")==0) {
        exit_shell();
    }
}

void tokenize_input(char *str){
    char* token;
    char delim[1] = " ";
    char ** tokenizedStr;
    token = strtok(str, delim);

    int i=0;

    while(token!=NULL){

         tokenizedStr[i] = token;
         i++;
         token = strtok(NULL,delim);

     }
    //i was incremented an extra time when for loop exited
    process_tokens(tokenizedStr,i--);
}

int readInput(){
    char * buff;
    char path[1024];
//    printf("%s",);
    buff = readline(getcwd(path, sizeof(path)));
    if(strlen(buff)!=0){
      if(fork()==0){
        tokenize_input(buff);
        exit(0);
      }
      free(buff);
      return 1;
    } else {
        return 0;
    }
}


int main(int argc, const char * argv[]) {
    char path[1024];
    getcwd(path, sizeof(path));
    strcat(path, "/myshell");
    setenv("shell", path, 1);
    parentprocess = getpid();

        while(1){
        readInput();
    }

    printf("sadfasdf");
    return 0;
}
