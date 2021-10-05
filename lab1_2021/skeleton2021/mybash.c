#include <stdio.h> 
#include <stdbool.h>

#include "command.h"
#include "execute.h"
#include "parser.h"
#include "builtin.h"
#include "prompt.h"

#include<unistd.h> 
#include <stdlib.h>
#include <limits.h>
#define MAX_HOST 100

int main(int argc, char *argv[]){
    Parser parser = NULL;
    pipeline pipe;
    bool quit = false;

    char * user;
    user = getlogin();
    char *hostname;
    hostname = calloc(MAX_HOST,sizeof(char));
    gethostname(hostname, 1000);
    char cwd[PATH_MAX];
    parser = parser_new(stdin);

    while (!quit) {
        //show_prompt();
        getcwd(cwd, sizeof(cwd));
        printf("\033[1;33m");
        printf("%s@%s:",user,hostname);
        printf("\033[0;36m");
        printf("%s>",cwd);
        printf("\033[0m");
        pipe = parse_pipeline(parser);
        quit = parser_at_eof(parser); /* Chequeo si hay que salir luego de ejecutar el comando */

        if (pipe != NULL) {
            quit = quit || builtin_is_exit(pipe);
            execute_pipeline(pipe);
            pipeline_destroy(pipe);
        } else if (!quit) {
            printf("Comando no valido\n");
        }
    }
    parser_destroy(parser); 
    parser = NULL;
    free(hostname);
    hostname = NULL;
    return 0;	
}
