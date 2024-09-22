#include "myshell.h"

// Kaushal Sambhe, 22388443
/* 
I declare that this material, which I now submit for assessment,
is entirely my own work and has not been taken from the work of others
save and to the extent that such work 
has been cited and acknowledged within the text of my work.
*/


/* 
********************************************************************
version: 1.0
date:    December 2003
author:  Ian G Graham
School of Information Technology
Griffith University, Gold Coast
ian.graham@griffith.edu.au
copyright (c) Ian G Graham, 2003. All rights reserved.
This code can be used for teaching purposes, but no warranty,
explicit or implicit, is provided.
*******************************************************************/
extern char **environ;


//Executes the commands
void commands(char * args[MAX_ARGS]){

    char *outputFile = NULL;
    int redirect = 0;
    for (int i = 0; args[i] != NULL; i++) {//Loop through the arguments
        if (strcmp(args[i], ">") == 0) {//Check if the argument is >
            args[i] = NULL;
            outputFile = args[i + 1];//Set the output file
            redirect = 1;//Flag for redirection
            break;
        }else if (strcmp(args[i], ">>") == 0) {//Check if the argument is >>
            args[i] = NULL;
            outputFile = args[i + 1];//Set the output file
            redirect = 2;//Flag for redirection
            break;
        }
    }
// O_WRONLY: This flag opens the file for write-only access.
// O_CREAT: This flag creates the file if it does not exist.
// O_TRUNC: This flag truncates the file's length to 0. If the file previously contained any data, it is lost.
// O_APPEND: This flag opens the file in append mode. Any data written to the file is added to the end regardless of the current file position.
//https://www.maths.cam.ac.uk/computing/linux/unixinfo/perms#:~:text=644%20means%20you%20can%20read,Suitable%20for%20public%20text%20files. <--- 0644 Permissions Information

//https://www.geeksforgeeks.org/dup-dup2-linux-system-call/


    int original_stdout = -1;
    if (redirect) {//If the command is a redirection command
        original_stdout = dup(1);//Duplicate the file descriptor
        int check_if_open = open(outputFile, redirect == 1 ? O_WRONLY | O_CREAT | O_TRUNC : O_WRONLY | O_CREAT | O_APPEND, 0644);//Open the file
        //0644 means you can read and write the file, and others can only read it
        if (check_if_open < 0) {
            perror("open failed");
            return;
        }
        dup2(check_if_open, 1);//Duplicate the file descriptor
        close(check_if_open);//Close the file
    }
    if (!strcmp(args[0],"clr")) {//Checks if first argument is clr
        clear_terminal(); 
    }
    else if (!strcmp(args[0],"quit")){//Checks if first argument is quit
        quit_process();
    }
    else if (!strcmp(args[0],"dir")){//Checks if first argument is dir
        dir(args);
    }
    else if (!strcmp(args[0],"pause")){//Checks if first argument pause
        pause_process();
    }
    else if (!strcmp(args[0],"environ")){//Checks if first argument is environ
        get_environs();
    }
    else if (!strcmp(args[0],"cd")){//Checks if first argument is cd
        change_directory(args);
    }
    else if (!strcmp(args[0],"echo")){//Checks if first argument is echo
        echo(args);
    }
    else if (!strcmp(args[0],"help")){//Checks if first argument is help
        help();
    }
    else {
        executeCommand(args);//If the command is not supported by the shell, call the executeCommand function to excute non-supported commands
    }

    if(redirect){
        fflush(stdout);//Flush the output buffer
        dup2(original_stdout, 1);//Duplicate the file descriptor
        close(original_stdout);//Close the file
    }
}

//Tokenizes the input
//https://stackoverflow.com/questions/65550867/tokenizing-string-input-using-a-function
void tokenizeInput(char* buffer, char** arguments) {
    char** currentArg = arguments;
    *currentArg++ = strtok(buffer, SEPARATORS);   // tokenize input

    while ((*currentArg++ = strtok(NULL, SEPARATORS)));//Tokenize the input
}

//Allows commands not supported by the shell to be executed
void executeCommand(char** args) {
    int background = 0;
    for (int i = 0; args[i] != NULL; i++) {
        if (strcmp(args[i], "&") == 0) {//Check if the argument is &
            args[i] = NULL; // Remove the & from the arguments
            background = 1;
            break;
        }
    }

    int pid = fork();//Fork the process
    if (pid < 0) {
        fprintf(stderr, "Fork failed\n");//If the fork fails, print an error message
        return;
    }

    if (pid == 0) {
        // Child process
        for (int i = 0; args[i] != NULL; i++) {
        if (strcmp(args[i], "<") == 0) {//Check if the argument is <
            args[i] = NULL;
            if (args[i + 1] == NULL) {
                fprintf(stderr, "No input file specified\n");//Print error message if no input file is specified
                exit(EXIT_FAILURE);//Exit the process
            }
            //https://www.ibm.com/docs/en/zos/2.4.0?topic=functions-access-determine-whether-file-can-be-accessed
            if (access(args[i + 1], F_OK) == -1) {//Check if the file exists or not 
                fprintf(stderr, "Input file does not exist\n");//Print error message if the file does not exist
                exit(EXIT_FAILURE);//Exit the process
            }
            FILE *file = freopen(args[i + 1], "r", stdin);//Open the file
            if (file == NULL) {
                perror("freopen failed");//Print error message if freopen fails
                exit(EXIT_FAILURE);//Exit the process
            }
        } else if (strcmp(args[i], ">") == 0 || strcmp(args[i], ">>") == 0) {//Check if the argument is > or >>
            args[i] = NULL;
            if (args[i + 1] == NULL) {
                fprintf(stderr, "No output file specified\n");//Print error message if no output file is specified
                exit(EXIT_FAILURE);//Exit the process
            }
            FILE *file = freopen(args[i + 1], strcmp(args[i], ">") == 0 ? "w" : "a", stdout);//Open the file
            if (file == NULL) {
                perror("freopen failed");//Print error message if freopen fails
                exit(EXIT_FAILURE);//Exit the process
            }
        }
        }
        if (execvp(args[0], args) < 0) {
            fprintf(stderr, "Exec failed\n");//If the exec fails, print an error message
            exit(1);//Exit the process
        }
    } else {
        // Parent process
        if (background) {//If the process is a background process, print the message
            printf("Started Background Process\n");//
        } else {
            waitpid(pid, NULL, 0); // Wait for child process to finish
        }
    }
}

int main (int argc, char ** argv)
{
    char buffer[MAX_BUFFER];                      // line buffer
    char * arguments[MAX_ARGS];                   // pointers to arg strings
    char  prompt[4096] ;                       // shell prompt
    FILE *input = stdin;                      // file pointer for input

    // Set shell environment variable- https://linux.die.net/man/3/getcwd
    char path[MAX_BUFFER];
    if (getcwd(path, sizeof(path)) != NULL) {
        strcat(path, "/myshell");//Set the shell environment variable
        setenv("shell", path, 1);//Set the shell environment variable
        setenv("parent", path, 1);//Set the parent environment variable
    } else {
        perror("getcwd() error");//Print error message if getcwd fails
    }

    if (argc > 1) {
        input = fopen(argv[1], "r");//Open the file
        if (!input) {
            fprintf(stderr, "Error opening file: %s\n", argv[1]);//Print error message if the file cannot be opened
            return 1;
        }
    }
    
    while (1) { // start an infinite loop
        if(input == stdin) {
            if (getcwd(prompt, sizeof(prompt)) != NULL) {//Get the current working directory
                if (isatty(fileno(stdout))){//Check if the file descriptor is associated with a terminal
                    fprintf(stdout, "%s> ", prompt);//Print the prompt
                }
            } else {
                perror("getcwd() error");//Print error message if getcwd fails
            }
        }

        if (!fgets (buffer, MAX_BUFFER, input )) { // read a line
            break; // if fgets returns NULL, break out of the loop
        }

        char *newline = strchr(buffer, '\n'); // find the newline character
        if (newline) *newline = '\0';//Replace the newline character with null character
        /* tokenize the input into args array */
        tokenizeInput(buffer, arguments);//Sends buffer and arguments to the tokenizeInput function

        if (arguments[0]) {
            commands(arguments);//Sends arguments to the commands function if the first argument is not null
        }
    }
    
    if(input != stdin) fclose(input); // close file if it was used
    return 0;
}