#include <stdio.h>
#include <stdlib.h>
#include <string.h>

size_t using;

char **readFile(FILE *fp){//creates array of strings out of fp
	char **arr=calloc(2,sizeof(char*));//create array of strings
	size_t arrLength = 2;
	using = 0;//current first free index of arr
	char startOfLine = fgetc(fp);
	while(startOfLine!=EOF){
		if(using >= arrLength){//double array size
			arrLength *= 2;
			arr = realloc(arr,arrLength*sizeof(char*));
			if(arr == NULL) perror("realloc error");
		}
		char *s2 = calloc(100,sizeof(char));
		char *s1 = calloc(101,sizeof(char));
		if(s2 == NULL || s1 == NULL) perror("malloc error");
		if(fgets(s2,100,fp) == NULL) perror("failed to read next line");
		s1[0]=(char)startOfLine;
		strcat(s1,s2);//concat first char of line with the rest
		free(s2);
		arr[using]=s1;
		using++;
		startOfLine = fgetc(fp);
	}
	return arr;
}

int compare(const void *a, const void *b){//compare function for qsort
	return strcmp(*(const char**)a, *(const char**)b);
}

char **sortArray(char **arr){//sorts array alphabetically
	qsort(arr,using,sizeof(char*),compare);
	return arr;
}

void delArray(char** arr){//frees the space reserved for the array
	int i;
	for(i=0;i<using;i++) free(arr[i]);
	free(arr);
}

int main(int argc, char* argv[]){
	char **arr= sortArray(readFile(stdin));
	int i;
	for(i=0;i<using;i++){
		if(fputs(arr[i],stdout) == EOF) perror("failed to print next line");
	}
	delArray(arr);
	return 0;
}
