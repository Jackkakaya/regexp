#include "core.h"
#include <stdio.h>
int main(int argc, char const * argv[])
{
	char regexpFile[] = "regexpFile.txt";
	char textFile[] = "textFile.txt";
	char* matches[MAXLENGTH];
	int a = regex_match(textFile,regexpFile,matches);
	printf("%d\n", a);
	for(int i=0;i<a;i++){
		printf("%s\n",matches[i]);
	}
	return 0;
} 