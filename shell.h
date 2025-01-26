#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <stdbool.h>
#include <fcntl.h>





char* readInput(void);

char** parseInput(char* line);

int execution(char** args);
/*command command(char **args);*/

int own_cd(char **args);
int own_exit(char **args);
int own_help(char **args);
int own_redirection(char **args);

#endif
