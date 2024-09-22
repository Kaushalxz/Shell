// #ifndef MYSHELL_H
// #define MYSHELL_H
// Kaushal Sambhe, 22388443
/* 
I declare that this material, which I now submit for assessment,
is entirely my own work and has not been taken from the work of others
save and to the extent that such work 
has been cited and acknowledged within the text of my work.
*/

//////////////////////////////////////////////////
#include <string.h>// for strcmp
#include <stdio.h>// for printf
#include <stdlib.h>// for exit
#include <sys/wait.h> // for wait
#include <unistd.h> // for chdir, getcwd
#include <fcntl.h> // for open

#define MAX_BUFFER 1024                        // max line buffer
#define MAX_ARGS 64                            // max # args
#define SEPARATORS " \t\n"                     // token separators
#ifndef MYSHELL_H                              // for ifndef
#define MYSHELL_H                              // for ifndef

extern char **environ;// for environ

void clear_terminal(void);//Clears the terminal
void quit_process(void);//Quits the shell
void dir(char** args);//Lists the contents of a directory
void pause_process(void);//Pauses the shell
void get_environs(void);//Prints the environment variables
void change_directory(char** args);//Changes the current directory
void echo(char** args);//Prints the arguments
void help(void);//Displays the help menu
void executeCommand(char** args);//Allows commands not supported by the shell to be executed
void tokenizeInput(char* buffer, char** arguments);//Makes standard input into tokens
void commands(char * args[MAX_ARGS]);// uses string comparision to check which command to execute and checks for redirection symbols

#endif