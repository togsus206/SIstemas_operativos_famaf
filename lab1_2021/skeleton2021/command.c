#include <stdio.h> 
#include <glib.h>
#include <assert.h>
#include <stdlib.h>
#include "tests/syscall_mock.h"
#include "command.h"

#include "strextra.h"


/********** COMANDO SIMPLE **********/

/* Estructura correspondiente a un comando simple.
 * Es una 3-upla del tipo ([char*], char* , char*).
 */

struct scommand_s {
    GSList *args;
    char * redir_in;
    char * redir_out;
};

scommand scommand_new(void){
	scommand new_command = (scommand)malloc(sizeof(struct scommand_s));
	if(new_command == NULL){
		printf("No se pudo reservar memoria");
		exit(-1);
	}
	else{
		new_command -> args = NULL;
		new_command->redir_in = NULL;
		new_command->redir_out = NULL;
    }
	assert(new_command != NULL && scommand_is_empty(new_command) && scommand_get_redir_in(new_command) == NULL && scommand_get_redir_out (new_command) == NULL);
    return new_command;    
}

/*cambiarlooo*/
scommand scommand_destroy(scommand self){
	assert(self != NULL);

	g_slist_free(self -> args);
	free(self -> redir_in);
	free(self -> redir_out);
	free(self);
	self -> redir_in = NULL;
	self -> redir_out = NULL;
	self = NULL;
	return self;
}

unsigned int scommand_length(const scommand self){
    assert(self != NULL);
	return g_slist_length(self->args);
}

bool scommand_is_empty(const scommand self){ 
	assert(self != NULL);
	return scommand_length(self) == 0;
}

void scommand_push_back(scommand self, char * argument){
	assert(self != NULL && argument != NULL);
	self -> args = g_slist_append(self->args,argument);
}


void scommand_pop_front(scommand self){
	assert(self != NULL && !scommand_is_empty(self));
	GSList * aux = NULL;
	aux = self->args;
	self->args = self->args->next;
	g_slist_free_1(aux);
	aux = NULL;
}



void scommand_set_redir_in(scommand self, char * filename){
	assert(self != NULL);
	self -> redir_in = filename;
}

void scommand_set_redir_out(scommand self, char * filename){
	assert(self != NULL);
	self -> redir_out = filename;
}

char * scommand_front(const scommand self){
	
	assert(self != NULL && !scommand_is_empty(self));
	return (char*)g_slist_nth_data(self->args, 0);
}


char * scommand_get_redir_in(const scommand self){
	assert(self != NULL);
	return (self->redir_in);
}


char * scommand_get_redir_out(const scommand self){
	assert(self != NULL);
	return (self->redir_out);
}


char * scommand_to_string(const scommand self)
{
	assert(self != NULL);
	char * result = NULL;
	if(!scommand_is_empty(self)){
		unsigned int size = scommand_length(self);
		result = strdup(g_slist_nth_data(self->args,0));
		for (unsigned int i = 1; i < size; i++)
		{
			result = strmerge(result, " ");
			result = strmerge(result,g_slist_nth_data(self->args,i));
		}
		if (self->redir_out) 
		{
			result = strmerge(result, " > ");
			result = strmerge(result,self->redir_out);
		}
		if (self->redir_in) 
		{
			result = strmerge(result, " < ");
			result = strmerge(result,self->redir_in);
		}
	}
	else{
		result = "";
	}
	assert(scommand_is_empty(self) ||  scommand_get_redir_in(self)==NULL || scommand_get_redir_out(self)==NULL ||  strlen(result)>0);
	return result;
}

/********** COMANDO PIPELINE **********/

/* Estructura correspondiente a un comando pipeline.
 * Es un 2-upla del tipo ([scommand], bool)
 */

struct pipeline_s {
    GSList *scmds;
    bool wait;
};


pipeline pipeline_new(void){
	pipeline new_pipeline = (pipeline)calloc(1,sizeof(struct pipeline_s));
	if(new_pipeline == NULL){
		printf("No se pudo reservar memoria");
		return NULL;
	}
	new_pipeline->scmds = NULL;
	new_pipeline->wait = true; 
	assert(new_pipeline != NULL && pipeline_is_empty(new_pipeline) && pipeline_get_wait(new_pipeline));
    return new_pipeline; 
}

pipeline pipeline_destroy(pipeline self){
	assert(self != NULL);

	g_slist_free(self -> scmds); 
	free(self);
	self = NULL;
	return self;
}


void pipeline_push_back(pipeline self, scommand sc){
	assert(self != NULL && sc != NULL);
	self->scmds = g_slist_append(self->scmds,sc);
}


void pipeline_pop_front(pipeline self){
	assert(self != NULL && !pipeline_is_empty(self));
	GSList * aux = NULL;
	aux = self->scmds;
	self->scmds = self->scmds->next;
	g_slist_free_1(aux);
	aux = NULL;
}

void pipeline_set_wait(pipeline self, const bool w){
	assert(self != NULL);
	self -> wait = w;
}

unsigned int pipeline_length(const pipeline self){
	assert(self != NULL);
	return g_slist_length(self -> scmds);
}

bool pipeline_is_empty(const pipeline self){
	assert(self != NULL);
	bool is_empty = pipeline_length(self);
	return is_empty==0;
}


scommand pipeline_front(const pipeline self){
	assert(self != NULL && !pipeline_is_empty(self));
	scommand aux = NULL;
	aux = g_slist_nth_data(self->scmds,0);
	assert(aux != NULL);
	return aux;
}

scommand pipeline_back(const pipeline self){
	assert(self != NULL && !pipeline_is_empty(self));
	scommand aux = NULL;
	aux = g_slist_nth_data(self->scmds,1);
	assert(aux != NULL);
	return aux;
}

bool pipeline_get_wait(const pipeline self){
	assert(self != NULL);
	return (self->wait);
}


char * pipeline_to_string(const pipeline self)
{   
	assert(self != NULL);
	if(!pipeline_is_empty(self)){
		char *temp = NULL;
		temp = scommand_to_string(g_slist_nth_data(self->scmds,0));
	 
		char *result = strdup(temp);
		//free(temp);
		temp = NULL;
		unsigned int size = pipeline_length(self);
		for (unsigned int  i = 1; i < size; i++)
		{
			result = strmerge(result, " | ");
			temp = scommand_to_string(g_slist_nth_data(self->scmds,i));
			result = strmerge(result,temp);
			//free(temp);
			temp = NULL;
		}

		if (!pipeline_get_wait(self)){
			result = strmerge(result," &");
		}
		return result;
	}
	else{
		return "";
	}
}


