#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "parser.h"

#define BUFLEN 1024

//To Do: This base file has been provided to help you start the lab, you'll need to heavily modify it to implement all of the features
extern char **environ;
int main() {
    char buffer[1024];
    char* parsedinput;
    char* args[4] = {NULL};
    char newline;
    
    printf("Welcome to the GroupXX shell! Enter commands, enter 'quit' to exit\n");
    do {
        //Print the terminal prompt and get input
        printf("$ ");
        char *input = fgets(buffer, sizeof(buffer), stdin);
        if(!input)
        {
            fprintf(stderr, "Error reading input\n");
            return -1;
        }
        
        //Clean and parse the input string
        parsedinput = (char*) malloc(BUFLEN * sizeof(char));
        size_t parselength = trimstring(parsedinput, input, BUFLEN);

        //Sample shell logic implementation
        if ( strcmp(parsedinput, "quit") == 0 ) {
            printf("Bye!!\n");
            return 0;
        }
        else {
            char * token;
            int arg_index = 0;
            token = strtok(parsedinput, " \"“");
            while (token != NULL && arg_index < 3){
                args[arg_index++] = token;
                token = strtok(NULL, " \"“");
            }
           
            pid_t pid = fork();
            if (pid < 0) {
                perror("fork");
                continue;
            }
            else if (pid ==0) {
                execve(args[0], args, NULL);
                perror("execve");
                _exit(1);
            }
            else {
                wait(NULL);
            }
            
        }

        //Remember to free any memory you allocate!
        free(parsedinput);
    } while ( 1 );

    return 0;
}
