#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>

#include "builtin.h"
#include "strextra.h"

#include <stdio.h>

bool builtin_is_exit(pipeline pipe){
	return strcmp(scommand_front(pipeline_front(pipe)),"exit")==0;
}

bool builtin_is_cd(pipeline pipe){
	return strcmp(scommand_front(pipeline_front(pipe)),"cd")==0;
}

bool builtin_is_internal(pipeline pipe){
	return builtin_is_exit(pipe) || builtin_is_cd(pipe);
}

void builtin_exec(pipeline pipe){
	assert(builtin_is_internal(pipe));
	
	if (!pipeline_is_empty(pipe)){
		if (builtin_is_cd(pipe)){
			if (scommand_length(pipeline_front(pipe)) < 2){
				printf("No se pudo ejecutar el comando\n");
				;
			}
			else{
				scommand aux = NULL;
			    aux = scommand_new();
			    aux = pipeline_front(pipe);
			    scommand_pop_front(aux);
	            chdir(scommand_front(aux));
			}

		}
		if (builtin_is_exit(pipe)){
			pipeline_destroy(pipe);
			exit(0);
		}
	}
}


