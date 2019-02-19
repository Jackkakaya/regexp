#ifndef CORE
#define CORE

#include "regex.h"
#include "fileio.h"

int regex_match(const char* textFile, const char* regexpFile, char** matches) {
	int count = 0;
	res = (Queue*)malloc(sizeof(Queue));
	create(res);
	char *textTable[MAXLENGTH];
	int textFilelength = readFile(textTable, textFile);
	char regex[MAXLENGTH]={'\0'};
	readRegex(regex, regexpFile);
	for (int i = 0; i < textFilelength; i++) {

		re_search(regex, textTable[i]);
		if (length(res) > 0) {
			matches[count] = (char*)malloc(length(res) * sizeof(char));
			int index = 0;
			while (length(res) > 0) {
				matches[count][index++] = gethead(res);
			}
			matches[count][index] = '\0';
			clear(res);
			count++;
		}
	}
	destroy(res);
	return count;
}


#endif