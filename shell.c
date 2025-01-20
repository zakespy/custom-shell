/* #include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<stdin.h> */
#include "shell.h"


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
	return (line);
}

char** parseInput(char* line){
		char **tokens = NULL;
		int token_count = 0;

		char *token = strtok(line," ");
		while(token != NULL){
			tokens = realloc(tokens,(token_count+1)*sizeof(char *));
			if(tokens == NULL){
				perror("realloc");
				free(line);
				return NULL;
			}

			tokens[token_count] = malloc(strlen(token)+1);
			if(tokens[token_count] == NULL){
				perror("malloc");
				free(line);
				for(int i=0;i<token_count;i++){
					free(tokens[i]);
				}
				free(tokens);
				return NULL;
			}

			strcpy(tokens[token_count],token);
			token_count++;

			token = strtok(NULL," ");
		
		}	
		for(int i=0;i<token_count;i++){
			printf("[%d]: %s\n",i,tokens[i]);
		}

		return (tokens);

}




int main(int argc,char **argv){
	
	(void)argc  ,(void)argv;
	char *line = NULL;
	/*size_t count = 0;
	ssize_t nread;*/
	pid_t child_pid;


	/* write(STDOUT_FILENO," Satyam Shell",14);
	*/
	int status = 1;
	while(status){
		printf("CShell -> ");

		/* read the command #########################################*/
		/*nread = getline(&line,&count,stdin);
		
		if(nread == -1)
		{
			perror("Exiting Shell");
			exit(1);
		}*/
		line = getInput();
		child_pid = fork();
		
		/*dividing the command into tokens ############################*/
		/*char **tokens = NULL;
		int token_count = 0;

		char *token = strtok(line," ");
		while(token != NULL){
			tokens = realloc(tokens,(token_count+1)*sizeof(char *));
			if(tokens == NULL){
				perror("realloc");
				free(line);
				return 1;
			}

			tokens[token_count] = malloc(strlen(token)+1);
			if(tokens[token_count] == NULL){
				perror("malloc");
				free(line);
				for(int i=0;i<token_count;i++){
					free(tokens[i]);
				}
				free(tokens);
				return 1;
			}

			strcpy(tokens[token_count],token);
			token_count++;

			token = strtok(NULL," ");
		
		}*/

		char **tokens = parseInput(line);

		/* execution ##########################################*/
		if(child_pid == -1){
			perror("failed to create ");
			exit(1);
		}
		if(child_pid == 0){
			/*for(int i=0;i<token_count;i++){
				printf("[%d]: %s\n",i,tokens[i]);
			}*/


			/*if(execvp(tokens[0],tokens) == -1){
				perror("Error");
			}*/

			/*printf("%s",line);*/
		}else{
			wait(NULL);
			printf("i am parent ");
		}
		/*for(int i=0;i<token_count;i++){
			free(tokens[i]);
		}*/
		free(tokens);
	}
	free(line);
	return 0;
}
