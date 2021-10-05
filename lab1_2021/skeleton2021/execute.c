#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "command.h"
#include "execute.h"
#include "tests/syscall_mock.h"
#include "builtin.h"

//

void execute_double_pipeline(pipeline apipe){

   scommand first;
   scommand second;
   char **argv;
   int infile;
   int outfile;
       
   int fd[2];
   int pid1, pid2;

   if(pipe(fd) < 0){
      printf("No se pudo ejecutar el pipe\n");
      exit(0);
   } 
   first = pipeline_front(apipe);
   pipeline_pop_front(apipe);
   second = pipeline_front(apipe);

   pid1 = fork();        
   if (pid1 < 0){
      printf("No se pudo crear el proceso hijo\n");
      exit(0);
   }
   if (pid1 == 0){        //ejecuto el primer hijo
      argv = calloc(sizeof(char *), scommand_length(first)+1);
      if (scommand_length(first) == 1) {
        argv[0] = (char *)scommand_front(first);
         scommand_pop_front(first);
      } else {
         for(unsigned int i = 0; i <= scommand_length(first)+1; i++){
           argv[i] = (char *)scommand_front(first);
           scommand_pop_front(first);
         }  
      }
      if(scommand_get_redir_in(first) != NULL) {
         infile = open((const char *)scommand_get_redir_in(first), O_RDONLY, S_IRUSR);
         dup2(infile, STDIN_FILENO);
         close(infile);
       }
      dup2(fd[1],STDOUT_FILENO);
      close(fd[1]);
      close(fd[0]);
      if (execvp(argv[0], argv) < 0){
         printf("Primer comando desconocido\n");
         exit(0);
      }
      exit(0);
   } else {
      pid2 = fork();   
      if(pid2 < 0){
         printf("No se pudo crear el proceso hijo\n");
         exit(0);
      } 
      if (pid2 == 0){
         argv = calloc(sizeof(char *), scommand_length(second)+1);
         if (scommand_length(second) == 1) {
            argv[0] = (char *)scommand_front(second);
            scommand_pop_front(second);
         } else {
            for(unsigned int i = 0; i <= scommand_length(second)+1; i++){
               argv[i] = (char *)scommand_front(second);
               scommand_pop_front(second);
            }
         }
         if(scommand_get_redir_out(second) != NULL){
            outfile = open((const char *)scommand_get_redir_out(second), O_WRONLY | O_CREAT, S_IRWXU);
            dup2(outfile , STDOUT_FILENO);
            close(outfile);
         }   
		 dup2(fd[0], STDIN_FILENO); 
         close(fd[0]);		
         close(fd[1]); 
         if(execvp(argv[0], argv) < 0){
            printf("Segundo comando desconocido\n");
            exit(0);
         } 
       } else {
         close(fd[0]);
         close(fd[1]);
         if(pipeline_get_wait(apipe)){
            wait(NULL);      
       }
        
    }
  }
}



void execute_pipeline(pipeline apipe){
    assert(apipe != NULL);
	
	int m = pipeline_length(apipe);
	if (m>2){
		printf("Demasiados comandos\n");
        exit(-1);
	}
    else{
    	//There are 2 pipelines
        if (m == 2){ 
        	execute_double_pipeline(apipe);
        }
        if (builtin_is_internal(apipe)){
			builtin_exec(apipe);           
        }
        else{
            int pid;
            scommand first = pipeline_front(apipe);
            char **argv;
            int infile;
            int outfile;

            if (first == NULL){
                printf("No se reconoce el comando\n");
            }

            argv = calloc(scommand_length(first)+1, sizeof(char *));      
            if (scommand_length(first) == 1) {
                argv[0] = (char *)scommand_front(first);
                scommand_pop_front(first);
            }
            else {
               for(unsigned int i = 0; i <= scommand_length(first)+1; i++){
                  argv[i] = (char *)scommand_front(first);
                  scommand_pop_front(first);
                }
            }

            pid = fork();
            if (pid == 0){
              if(scommand_get_redir_in(first) != NULL){
                  infile = open((const char *)scommand_get_redir_in(first), O_RDONLY, S_IRUSR);
                dup2(infile, STDIN_FILENO);
                close(infile);
              }
              if (scommand_get_redir_out(first) != NULL){
                   outfile = open((const char *)scommand_get_redir_out(first), O_WRONLY | O_CREAT, S_IRWXU );
                   dup2(outfile , STDOUT_FILENO);
                   close(outfile);
               }

               if(execvp(argv[0], argv) < 0){
                      printf("%s : COmando desconocido\n",argv[0]);
                      exit(1);
                }
            }
            else {
                if(pipeline_get_wait(apipe)){
                      wait(NULL);
                }
            }
        }
    }	
}