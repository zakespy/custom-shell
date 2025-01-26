/* #include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<stdin.h> */
#include "shell.h"
#include <fcntl.h>


typedef enum{
	CD,
	EXIT,
	HELP,
	REDIRECTION,
	UNKNOWN
} command;


command getCommand(char**args){
	// printf("%s \n",args[1]);
	if (strcmp(args[0],"cd") == 0) return CD;
	if (strcmp(args[0],"exit") == 0) return EXIT;
	if (strcmp(args[0],"help") == 0) return HELP;
	if (args[1] != NULL && (strcmp(args[1],"<") == 0 || strcmp(args[1],">") == 0)) return REDIRECTION;
	return UNKNOWN;
}



char* getInput(void){
	char *line = NULL;
	size_t count = 0;
	ssize_t nread = getline(&line,&count,stdin);

	if(nread == -1)
	{	
		free(line);
		perror("Exiting Shell");
		exit(1);
	}
	
	if(line[nread-1] == '\n')
		line[nread-1] = '\0';

	return (line);
}

char** parseInput(char* line){
		// char **tokens = NULL;
		// int token_count = 0;
		// char *token = strtok(line," ");

		// while(token != NULL){
		// 	tokens = realloc(tokens,(token_count+1)*sizeof(char *));
		// 	if(tokens == NULL){
		// 		perror("realloc");
		// 		/*free(line);*/
		// 		return NULL;
		// 	}

		// 	tokens[token_count] = strdup(token);
		// 	if(tokens[token_count] == NULL){
		// 		perror("malloc");
		// 		/*free(line);*/
		// 		for(int i=0;i<token_count;i++){
		// 			free(tokens[i]);
		// 		}
		// 		free(tokens);
		// 		return NULL;
		// 	}
			
		// 	token_count++;

		// 	token = strtok(NULL," ");
		
		// }
		// tokens[token_count] = NULL;
		
		// return (tokens);


	char** tokens = NULL;
    int token_count = 0;
    char* token = strtok(line, " ");
    
    while (token != NULL) {
        char** temp = realloc(tokens, (token_count + 1) * sizeof(char*));
        if (temp == NULL) {
            perror("realloc failed");
            for (int i = 0; i < token_count; i++) {
                free(tokens[i]);
            }
            free(tokens);
            return NULL;
        }
        tokens = temp;

        tokens[token_count] = strdup(token);
        if (tokens[token_count] == NULL) {
            perror("strdup failed");
            for (int i = 0; i < token_count; i++) {
                free(tokens[i]);
            }
            free(tokens);
            return NULL;
        }

        token_count++;
        token = strtok(NULL, " ");
    }

    tokens = realloc(tokens, (token_count + 1) * sizeof(char*));  // Ensure space for NULL terminator
    if (tokens == NULL) {
        perror("realloc failed");
        return NULL;
    }

    tokens[token_count] = NULL;  // Null terminate
    return tokens;

}


int execution(char **args){
	
	// build in function
	switch( getCommand(args) ){
		case CD:
			return own_cd(args);
			break;

		case EXIT:
			return own_exit(args);
			break;

		case HELP:
			return own_help(args);
			break;

		case REDIRECTION:
			return own_redirection(args);
			break;

		case UNKNOWN:
			break;

		default:
			break;
	}
	
	/* for non built in function */ 
        pid_t child_pid = fork();
 	if (child_pid == -1) {
            perror("failed to create child process");
            free(args);
            exit(1);
	    return -1;
        }

        if (child_pid == 0) {
            if (execvp(args[0], args) == -1) {
                perror("Error executing command");
            }
            exit(1);
        } else {
            wait(NULL); 
        }


	return 1;
}


int own_cd(char **args){
	chdir(args[1]);
	return 1;
}

int own_help(char **args){
	printf("Custom shell help:\n");
    printf("************************************\n");
    printf("1) ls :- List directory contents\n");
    printf("	Displays a list of files and directories in the current directory or specified directories.\n");
    printf("\n");    
    printf("2) cd :- Change directory\n");
    printf("	Changes the current directory to the specified directory.\n");
    printf("\n");
    printf("3) exit :- Exit the shell\n");
    printf("	Exits the shell and terminates the program.\n");
	printf("************************************\n");
	return 1;
}

int own_exit(char** args){
 	return 0;
}

int own_redirection(char **args){
	// printf("%s %s %s\n",args[0],args[1],args[2]);
	char* target = NULL;
	char* source = NULL;
	if(args[0] == NULL || args[2] == NULL){
		perror("provide the source and target file");
		return -1;
	}
	if(strcmp(args[1],"<") == 0){
		// printf(" target < source \n");
		target = args[0];
		source = args[2];
	}
	else if(strcmp(args[1],">") == 0 ){
		// printf("source > target \n");
		target = args[2];
		source = args[0];
	}

	pid_t pid  = fork();


	if(pid == -1){
		perror("Fork error");
		return -1;
	}

	if(pid == 0){
		int out = open(target, O_RDWR|O_CREAT|O_APPEND, 0600);
    	if (out == -1){
			perror("opening cout.log"); 
			return -1; 
		}

    	int save_out = dup(fileno(stdout));
    	if (dup2(out, fileno(stdout)) == -1) { 
			perror("cannot redirect stdout"); 
			return -1; 
		}

		if(execvp(source,args) == -1){
			perror("problem with source file");
			return -1;
		}

    	fflush(stdout); close(out);
    	dup2(save_out, fileno(stdout));
    	close(save_out);

	}else{
		wait(NULL);
	}

	

    // int err = open("cerr.log", O_RDWR|O_CREAT|O_APPEND, 0600);
    // if (-1 == err) { perror("opening cerr.log"); return -1; }
    // int save_err = dup(fileno(stderr));
    // if (-1 == dup2(err, fileno(stderr))) { perror("cannot redirect stderr"); return -1; }
    // puts("doing an ls or something now");
    // fflush(stderr); close(err);
    // dup2(save_err, fileno(stderr));
    // close(save_err);


	
	return 1;

}

int main(int argc, char **argv) {
    (void)argc;
    (void)argv;

    while (1) {
        printf("CShell -> ");
        char *line = getInput(); 

        if (line == NULL) { 
			printf("Error: Null input");
            break; 
        }

        char **tokens = parseInput(line);

        if (tokens == NULL) {
            free(line);
            continue; 
        }

		int status = execution(tokens);
		if(status == 0){
			free(line);
			free(tokens);
			break;/* exit condition*/
		}
	/*if(!status)
		break;*/
        free(line); 
		int count =0;
		while(tokens[count] != NULL){
			free(tokens[count]);
			count++;
		}
        free(tokens); 
		// break;

    }
	
    return 0;
}
