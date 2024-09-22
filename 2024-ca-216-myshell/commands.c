// Kaushal Sambhe, 22388443
/* 
I declare that this material, which I now submit for assessment,
is entirely my own work and has not been taken from the work of others
save and to the extent that such work 
has been cited and acknowledged within the text of my work.
*/

//////////////////////////////////////////////////

#include "myshell.h"

void clear_terminal() {
    system("clear");
}

//Quits the shell
void quit_process() {
    exit(0);
}

//Lists the contents of a directory
void dir(char** args) {
    char *cmd[] = {"ls", "-al", args[1], NULL};// List all files in the directory
    int pid = fork();
    if (pid == 0) {
        // Child process
        execvp(cmd[0], cmd);
        exit(EXIT_FAILURE); // exec only returns on error
    } else if (pid < 0) {
        // Failed to fork
        perror("fork failed");// Print error message
    } else {
        // Parent process
        wait(NULL); // Wait for child process to finish
    }
}
//Pauses the shell
void pause_process() {
    printf("Press Enter to continue...");
    while (getchar() != '\n');
}


//Prints the environment variables
void get_environs() {
    for (int i = 0; environ[i] != NULL; i++){
        printf("%s\n",environ[i]);
    }
}


//Changes the current directory
void change_directory(char** args) {
    if (args[1]) {
        if (chdir(args[1]) < 0) {
            perror("chdir failed");// Print error message
        }
    } else {
        fprintf(stderr, "cd: missing operand\n");// Print error message
    }
}
//Prints the arguments
void echo(char** args) {
    for (int i = 1; args[i] != NULL; i++){
        printf("%s ", args[i]);
    }
    printf("\n");//
}

//Prints the help file
void help() {
    char *args[] = {"more", "readme", NULL};
    if (fork() == 0) {
        execvp(args[0], args);// Execute the command
    } else {
        wait(NULL);
    }
}