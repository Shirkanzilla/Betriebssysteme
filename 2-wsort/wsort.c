#include <stdio.h>
#include <stdlib.h>
#include <string.h>

size_t using;

char **readFile(FILE *fp){
	char **arr=malloc(1*sizeof(char*));
	size_t arrLength = 2;
	using = 0;
	char startOfLine = fgetc(fp);
	while(startOfLine!=EOF){
		if(using >= arrLength){
			printf("Before: %lu\n",arrLength);			
			arrLength += 1;
			arr = realloc(arr,arrLength*sizeof(char*));
			if(arr == NULL) printf("%s\n","realloc error");
			printf("arr: %p\n",(void*)arr);
			printf("After: %lu\n",arrLength);
		}
		char *s2 = malloc(100*sizeof(char));
		if(s2 == NULL) printf("%s\n","malloc error");
		fgets(s2,100,fp);
		char *s1 = malloc(sizeof(char));
		if(s1 == NULL) printf("%s\n","malloc error");
		s1[0]=(char)startOfLine;
		strcat(s1,s2);
		arr[using]=s1;
		using++;
		startOfLine = fgetc(fp);
	}
	return arr;
}
int main(int argc, char* argv[]){
	char **arr= readFile(stdin);
	int i;
	printf("using: %lu\n", using);
	printf("arr: %p\n",(void*)arr);
	for(i=0;i<using;i++){
		printf("i: %d\n",i);
		fputs(arr[i],stdout);
	}
	free(arr);
	return 0;
}
