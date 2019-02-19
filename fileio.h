#ifndef FILEIO
#define FILEIO 

#define MAXLENGTH 10000
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
int countLines(const char* fileName) {
	FILE* file = fopen(fileName, "r");
	if (file == NULL) {
		perror("Unable to find the file");
		exit(0);
	}
	int lines = 0;
	while (!feof(file))
	{
		int ch = fgetc(file);
		if (ch == '\n') { lines++; }
	}
	fclose(file);
	return ++lines;
}
int size(char* arr);

void readRegex(char* regex, const char* regexpFile) {
	FILE* file = fopen(regexpFile, "r");
	char tempchar[MAXLENGTH]={'\0'};
	fscanf(file, "%s", tempchar);
	int index=0;
	for(int i=0;i<size(tempchar);i++){
		char temp = tempchar[i];
		if(temp=='\\'){
			i++;
			if(tempchar[i]=='w') temp='\a';
			else if(tempchar[i]=='d') temp = '\b';
			else if(tempchar[i]=='s') temp = '\v';
		}
		regex[index++] = temp;
	}
	fclose(file);
}

int readFile(char** textTable, const char* textFile) {
    int numLines = countLines(textFile);
	FILE* file = fopen(textFile, "r");
	int i = 0;
	for (; i < numLines; i++) {
		textTable[i] = (char*)malloc(MAXLENGTH * sizeof(char));
		if (fgets(textTable[i], MAXLENGTH, file) == NULL) {
			strcpy(textTable[i], "");
		}
		else {
			textTable[i][strcspn(textTable[i], "\n")] = 0;
		}
	}
	fclose(file);
	return numLines;
}

#endif