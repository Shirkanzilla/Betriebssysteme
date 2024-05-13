#include <stdio.h>
#include <stdlib.h>
#include <string.h>

size_t using;

char **readFile(FILE *fp){
	char **arr=calloc(1,sizeof(char*));
	size_t arrLength = 2;
	using = 0;
	char startOfLine = fgetc(fp);
	while(startOfLine!=EOF){
		if(using >= arrLength){
			arrLength *= 2;
			arr = realloc(arr,arrLength*sizeof(char*));
			if(arr == NULL) perror("realloc error");
		}
		char *s2 = calloc(100,sizeof(char));
		char *s1 = calloc(101,sizeof(char));
		if(s2 == NULL || s1 == NULL) perror("malloc error");
		if(fgets(s2,100,fp) == NULL) perror("failed to read next line");
		s1[0]=(char)startOfLine;
		strcat(s1,s2);
		free(s2);
		arr[using]=calloc(101,sizeof(char*));
		arr[using]=s1;
		//free(s1);
		using++;
		startOfLine = fgetc(fp);
	}
	return arr;
}

int compare(const void *a, const void *b){
	return strcmp(*(const char**)a, *(const char**)b);
}

char **sortArray(char **arr){
	qsort(arr,using,sizeof(char*),compare);
	return arr;
}

void delArray(char** arr){
	int i;
	for(i=0;i<using;i++) free(arr[i]);
	free(arr);
}

int main(int argc, char* argv[]){
	//FILE *fp = fopen("wlist0","r");
	//if(fp == NULL) {
      	//	perror("Error in opening file");
      	//	return(-1);
   	//}
	char **arr= sortArray(readFile(stdin));
	int i;
	for(i=0;i<using;i++){
		if(fputs(arr[i],stdout) == EOF) perror("failed to print next line");
	}
	delArray(arr);
	return 0;
}
