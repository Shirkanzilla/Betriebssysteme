#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <linux/limits.h>
#include <sys/wait.h>
#include "plist.h"
static int numOfProc;//maximum number of zombies

static int changeDirectory(char **tokenized){
	if(tokenized[1] == NULL || tokenized[2] != NULL){
		printf("usage: cd <directory>\n");
	}
	if(chdir(tokenized[1]) == -1) return 1;
	return 0;	
}

//callback function that prints pid and commandLine of every existing child process
static bool printCallback(pid_t pid, const char *commandLine){
	printf("%d %s\n", (int) pid, commandLine);
	return true;
}


static int executeCommand(char *fullCommand, char **tokenized, int using){
	bool (*ptr)() = &printCallback;
	//check if cd or jobs is called
    	if(strcmp(tokenized[0],"cd") == 0) return changeDirectory(tokenized);
	if(strcmp(tokenized[0],"jobs") == 0){ 
		walkList(ptr);
		return 0;
	}
	//executes command in a child process
	pid_t p = fork();
	int status;
	if(p == -1){
	perror("fork failed");
		exit(1);
	}else if(p == 0){
		if(strcmp(tokenized[using-1],"&") == 0){
			tokenized[using-1] = NULL;
		}
		tokenized[using] = NULL;
		if(execvp(tokenized[0],tokenized) == -1){
			perror("error while executing command");
			exit(EXIT_FAILURE);
		}
	}else{
		if(strcmp(tokenized[using-1],"&") == 0){
			numOfProc++;
			int retVal = insertElement(p,fullCommand);
			if(retVal < 0) return retVal;
			return 0;
		}else{
			waitpid(p, &status, 0);
			if(WIFEXITED(status)){
				printf("Exitstatus [%s] = %d\n", fullCommand, (int)WEXITSTATUS(status));
			}else return 1;
		}
	}	
	return 0;
}

//checks if any child has become a zombie and if so prints the exitstatus and removes it from the list
static int printBackgroundStatus(){
	int status;
	int finished = 0;
	for(int i = 0; i < numOfProc; i++){	
		pid_t p = waitpid(-1,&status,WNOHANG);
		if(p == 0 || errno==10) break;
		if(WIFEXITED(status)){
			finished++;
			char fullCommand[1337/sizeof(char)];
			removeElement(p, fullCommand, sizeof(fullCommand));
			printf("Exitstatus [%s] = %d\n", fullCommand, (int)WEXITSTATUS(status));
		}else{
		       	numOfProc -= finished;
			return 1;
		}
	}
	numOfProc -= finished;
	return 0;
}

int main(int argc, char *argv[]){
	numOfProc = 0;
	char **tokenized;
	while(1){	
		if(printBackgroundStatus()!=0){      
			return 1;
		}
		char cwd[PATH_MAX];
		if(getcwd(cwd,sizeof(cwd))==NULL){
			perror("getcwd() error");
			return 1;
		}
		printf(cwd);
		printf(": ");
		//get next input line
		char command[1337/sizeof(char)];
		if(fgets(command,sizeof(command),stdin)==NULL){
			if(feof(stdin)) break;
			perror("error reading next line");
			return 1;
		}
		//check if maximum length is exceeded
		if(strchr(command,'\n') == NULL){
			printf("warning: maximum input length exceeded");
			continue;
		}
		//save full input for later display:
		char fullCommand[1337/sizeof(char)];
		strcpy(fullCommand,command);
		fullCommand[strcspn(fullCommand,"\n")] = '\0';
		//create array of tokens out of input:
		tokenized = calloc(1337,1);
		if (tokenized == NULL){
			perror("malloc failed");
			free(tokenized);
			return 1;
		}
		int using = 1;
		char *token = strtok(command," \t\n");
		tokenized[0] = token;
		while(token != NULL){
			token = strtok(NULL," \t\n");
			tokenized[using] = token;
			using++;
		}
		using--;
		if(tokenized[0] == NULL){
			free(tokenized);
			continue;
		}
		if(tokenized == NULL){
			perror("realloc failed");
			free(tokenized);
			return 1;
		}
		executeCommand(fullCommand, tokenized, using);
		free(tokenized);
	}		
	return 0;	
}

