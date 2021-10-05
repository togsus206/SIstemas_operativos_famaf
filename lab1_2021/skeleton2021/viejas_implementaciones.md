/*/Ejecucion doble pipeline

void execute_double_pipeline(pipeline apipe){
    uint n1,n2,k1,k2;
    int fd[2];	
	
	char * com1 = scommand_front(pipeline_front(apipe));	
	char * com2 = scommand_front(pipeline_back(apipe));
        	
  	char * chain1[10];
   	char * chain2[10];
   	for (uint i = 0; i<10; i++){
        chain1[i]=NULL;
        chain2[i]=NULL;
    }
        	
    n1 = scommand_length(pipeline_front(apipe));
    n2 = scommand_length(pipeline_back(apipe));
        	
	k1 = n1;
	k2 = n2;
	if(n1>1){
		scommand_pop_front(pipeline_front(apipe));
	}
	if(n2>1){
		scommand_pop_front(pipeline_back(apipe));
	}
	for(uint j = 0;j<n1;j++){
		chain1[j]=scommand_front(pipeline_front(apipe));
	    if (k1>2){
	    	scommand_pop_front(pipeline_front(apipe));
	        k1 = scommand_length(pipeline_front(apipe));
	    }
	}
	for(uint j = 0;j<n2;j++){
		chain2[j]=scommand_front(pipeline_back(apipe));
		if (k2>2){
	    	scommand_pop_front(pipeline_front(apipe));
	        k2 = scommand_length(pipeline_front(apipe));
	    }
	}
	        
	if(pipe(fd) == -1){
		printf("Fallo el pipe\n");
	}
	        
	if (builtin_is_internal(apipe)){
		builtin_exec(apipe);        
   	}
    int pid1 = fork();
    if (pid1 < 0){
        printf("No se creo el proceso\n");
    }
    if (pid1 == 0){
        dup2(fd[1],STDOUT_FILENO);
        close(fd[0]);
        close(fd[1]);
        execvp(com1,chain1);
    }
    int pid2 = fork();
    if (pid2 < 0){
        printf("No se creo el proceso\n");
    }
    if (pid2 == 0){
	    dup2(fd[0],STDIN_FILENO);
        close(fd[0]);
        close(fd[1]);
        execvp(com2,chain2);
    }
  	close(fd[0]);
    close(fd[1]);
    waitpid(pid1,NULL,0);
    waitpid(pid2,NULL,0);
}

*/

//
/*/

//Funcion principal
void execute_pipeline(pipeline apipe){
    assert(apipe != NULL);
    bool have_w =pipeline_get_wait(apipe);
    pid_t pid;
    uint n,m,k;
	
	m = pipeline_length(apipe);
	if (m>2){
		printf("Demasiados comandos\n");
        exit(-1);
	}
    while(m>0){
    	//There are 2 pipelines
        if (m == 2){ 
        	execute_double_pipeline(apipe);
            break;
        }
        if (builtin_is_internal(apipe)){
			builtin_exec(apipe);
            break;           
         }
        else{
            pid = fork();
            if (pid == -1){
                printf("No se pudo crear el proceso hijo.\n");
            }
            else if (pid == 0){	
				char * chain[600];
				char * comando = scommand_front(pipeline_front(apipe));
            
                for (uint i = 0; i<600; i++){
                    chain[i] = NULL;
                }
                n = scommand_length(pipeline_front(apipe));   
                if(n>1){
                	scommand_pop_front(pipeline_front(apipe)); 
                }
                k = n;
                for (uint j = 0; j<n;j++){
                    chain[j] = scommand_front(pipeline_front(apipe));	
					if (k>2){ //Si tiene mas de dos argumentos que vaya revisando cada uno
						scommand_pop_front(pipeline_front(apipe));
						k = scommand_length(pipeline_front(apipe));
					}
                }
                
                //Redireccionamiento
                redicc_in(pipeline_front(apipe));
                redicc_out(pipeline_front(apipe));
                //


                if (execvp(comando,chain)<0){
                	printf("Comando desconocido\n");
                	exit(0);
                }          
            }
            else{
                if(have_w){
                    wait(NULL);
                }
            }
        
        }
        m = m-1;	
    }
}


//Funcion auxiliar para redireccionar

void redicc_in(scommand self){
        scommand first = self;
        int infile;
        int fd[2];
        fd[0] = 0;
        fd[1] = 0;
        char * redir = scommand_get_redir_in(first);
        if(redir != NULL) {
            infile = open((const char *)redir, O_RDONLY, S_IRUSR);
            dup2(infile, STDIN_FILENO);
            close(infile);
        }
        dup2(fd[1],STDOUT_FILENO);
        close(fd[1]);
        close(fd[0]);
}

void redicc_out(scommand self){
        scommand first = self;
        int outfile;
        int fd2[2];
        fd2[0] = 0;
        fd2[1] = 0;
        char * o_redir = scommand_get_redir_out(first);
        if (o_redir != NULL){
            outfile = open((const char *)o_redir, O_WRONLY | O_CREAT, S_IRWXU);
            dup2(outfile , STDOUT_FILENO);
            close(outfile);
        }
        dup2(fd2[0], STDIN_FILENO); 
        close(fd2[0]);		
        close(fd2[1]);
}

*/