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
#include<fcntl.h>
#include<unistd.h>
#include<sys/types.h>
#include <dirent.h>
#include<sys/wait.h>
#include<readline/readline.h>
#include<readline/history.h>
#include "myshell.h"

pid_t parentprocess;
extern char **environ;
void tokenize_input(char *str);
//void readFromFile(char** tokens);

//in-built command to exit the shell, kills parent process
void exit_shell(){
    //exit_flag=1;
    kill(parentprocess,SIGKILL);
}

//in-built command to clear the screen
void clr() {
  printf("\033[H\033[J");
}

//in-built command to pause the shell
void pause_shell() {
    printf("Shell paused, press enter to continue.");
    getchar();
}

//in-built command to list current directory contents
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

//in-built command to echo message back to stdout
void echo(char **tokens, int size){
    for(int i=1;i<size;i++){
       printf("%s ",tokens[i]);
    }
    printf("\n");
}

//in-built command to print environment variables to stout
void env() {
    char *ptr = *environ;

    for (int i = 1; i < ptr; i++) {
        printf("%s\n", ptr);
        ptr = *(environ+i);
    }
}

//in-built command to print manual to stdout
void help() {
    system("more README.md");
}

//in-built command to change current working directory to the one specified in argument
void change_directory(char **directory){
    //If no directory specified, obtain current working directory from system
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


//in-built function to open input file and redirect stdin to the file
void redirect_input(char **inFileName) {

  int fd_in;
  //Open file check for errors opening
  if((fd_in = open(inFileName, O_RDONLY, 0)) < 0) {
    printf("Error opening input file.");
    exit(0);
  }
  //Copy stdin to file
  dup2(fd_in, STDIN_FILENO);
  //Close the file descriptor for the redirect file, because that file is under the stdin descriptor now
  close(fd_in);
}

//in-built function to open output file and redirect stdout to the file
void redirect_output(char **outFileName) {

  int fd_out;
  if((fd_out = creat(outFileName, 0644)) < 0) {
    printf("Error opening output file.");
    exit(0);
  }
  //Copy stdout to file
  dup2(fd_out, STDOUT_FILENO);
  //Close the file descriptor for the redirect file, because that file is under the stdout descriptor now
  close(fd_out);
}


//Function to process user supplied tokens to corresponding in-built commands

void process_tokens(char *tokens[], int size) {
    //If command ends with ampersand, run command in the background
    if(strcmp(tokens[size-1], "&")==0) {

        int pid = fork();
        if(pid==0){
            setpgid(0,0);
            printf("Background process %d running %s.\n", getpid(), tokens[0]);
        } else {
            wait(NULL);
        }
    }  
    

    //If command is cd, change directory
    if(strcmp(tokens[0],"cd")==0){

        change_directory(tokens);
    //If command is clr, clear the screen
    } else if (strcmp(tokens[0],"clr")==0) {
        clr();
    //If command is dir, list the contents of the current directory
    } else if (strcmp(tokens[0],"dir")==0) {
        directory(tokens);

    //If command is environ, list the environment variables
    }else if (strcmp(tokens[0],"environ")==0) {
        env();
    //If command is echo, print back to stdout the message following echo
    }else if (strcmp(tokens[0],"echo")==0) {
       echo(tokens,size);
    //If command is help, print manual in README to stdout
    }else if (strcmp(tokens[0],"help")==0) {
        help();
    //If command is pause, pause the shell
    }else if (strcmp(tokens[0],"pause")==0) {
        pause_shell();
    //If command is quit, quit the shell
    }else if (strcmp(tokens[0],"quit")==0) {
        exit_shell();

    //If command is anything different from the above, print the message and do nothing otherwise
    }else {
        tokens[size++] = NULL;
         int pid = fork();
        if(pid==0){
             execvp(tokens[0],tokens);
             perror("execv");
        }
    }
}

void readFromFile(char* tokens){
   printf("%s file ",tokens);
    FILE* fp;
    char line[1024];
    fp = fopen(tokens, "r+");
    while (fgets(line, sizeof(line), fp)){
        int pid = fork();
        if(pid==0){
    printf("%s \n", line);
        tokenize_input(line);
        }else{
            wait(NULL);
        }
   }
    fclose(fp);
}

//Utility function to tokenize user input on white space
void tokenize_input(char *str){
//     printf("%s test test \n", str);
    char* token;
    //delimiter is white space
    char delim[1] = " ";
    char *tokens[3];
    token = strtok(str, delim);
    int in=0, out=0;
    char inFileName[50], outFileName[50];

    int size=0;
    while(token!=NULL){

         tokens[size] = token;
         size++;
         token = strtok(NULL,delim);

     }

     //Check of there is < or > in the tokenized user command
     for(int i = 0; i < size-2; i++) {
       if(strcmp(tokens[i], "<") == 0) {
         tokens[i]=NULL;
         strcpy(inFileName, tokens[i+1]);
         in=2;
       }
       if(strcmp(tokens[i], ">") == 0) {
         tokens[i]=NULL;
         strcpy(outFileName, tokens[i+1]);
         out=2;
       }
       //Invoke function to redirect accordingly
       if(in) {
         redirect_input(inFileName);
       }
       if(out) {
         redirect_output(outFileName);
       }
     }
     process_tokens(tokens,size--);


}

//Read user input from stdin, pass it to tokenizing function
int readInput(){
    char * buff;
    char path[1024];
    //Display current working directory in the command line
    buff = readline(strcat(getcwd(path, sizeof(path)),"/myShell $ "));
    //Pass input to tokenizing function
    if(strlen(buff)!=0){
        tokenize_input(buff);
        free(buff);
        return 1;
    } else {
        return 0;
    }
}

//Driver function
int main(int argc, const char * argv[]) {
    char path[1024];
    //Get current working directory from system settings
    getcwd(path, sizeof(path));
    strcat(path, "/myShell");
    setenv("shell", path, 1);
    parentprocess = getpid();
    
    if(argc>1) {
         
         readFromFile(argv[1]);
    } else {
        //Read input from user indefinitely, until user quite the shell explicitly
           while(1){
               readInput();
           }
    }

   

    return 0;
}