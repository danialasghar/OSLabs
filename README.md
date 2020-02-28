# Shell Readme

## What is a Shell?
* The *shell* is a program that provides an easy interface to control the computer using commands entered via the keyboard. It can be accessed by starting the programs: *terminal* in Linux & Mac and *powershell* in Windows. Once opened, the program will display a blinking cursor; awaiting user input. 

## Standard shell use commands
* In order to clear the shell screen for a cleaner UI run `clr`.
* If you want to display something as an output on the shell `echo` can be used followed by what needs to be output.
* To get help one can get `help` can be used.
* If you want to pause the operation of a command `pause` can be used.
* To exit from the shell run the command `quit`.
* If the command run is behaving incorrectly; it can be force quit using a signal `Control + C`

## Traversing the Filesystem
* To list the files in the current folder run the following command:
    `dir`
* To get the current folder the user is currently working in:
    `pwd`
* In order to change the folder the following command is used:
    `cd`
    * Example: `cd /home`
* The current folder you are working in is always considered to be "." ... As such, in order to go to parent or children folders, the "." is used as a reference point
    * Go up one folder
        `cd ..`
    * Go up two folders
        `cd ../..`
    * Go to a child folder in current folder 
        `cd Lab 2/`
* The folder you start the terminal with is usually is your *\home* folder which can be also referred to as `~` 


## Environment Variables
*  An environment variable is a dynamic-named value that can affect the way running processes will behave on a computer. They are part of the environment in which a process runs. 
* To see the existing variables in the system run the following
`environ`

## Arguments
* Most commands in the shell can take argument flags which will change the output of the command. For example, *ls* can take the arguments *-l*.
* Most shells will have a manual provided which can be used to search function definitions for the commands. The following will open the manual for the change direcotry command:
    `man cd`
* Multiple arguments can be passed into the shell using further arguments, seperated by a whitespace (space character or tab). For example:
    `programname argument1 argument 2 ...`

## Input & Output
* Shells can be used to input files as input and also for writing the output into a file. This can be done as follows `programname arg1 arg2 <inputfilename> <outputfilename>`. The shell will recognize the file names and automatically assume redirection of input and output. 
* This functionaly can extend using a batch file, a txt file used for automation with commands listed on seperate lines, to use the terminal as batch operator follow the format `myShell batchfilename`

## Run terminal in Background
* When a program is run from the terminal, they are attached together in a manner which causes the followign 2 issues:
    * Your terminal displays a constant stream of output and diagnostic data ... making it unusable, unless you close the program. 
    * If the terminal is closed, the program associated with also close automatically. 
* To fix this issue, add an "&" character at the end of the the command, example: `programname &`

