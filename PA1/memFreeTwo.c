#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void test(char *str){
	printf("%s\n", str);
}

int main(int argc, char **argv) {
	int i;	
	char *delims;
	char *delimTwo;
	char delimThree[100];

	delimThree[0] = 'h';
	delimThree[1] = 'i';
	
	printf("%s\n", delimThree);

	delims = malloc(sizeof(char)* 100);
	delimTwo = malloc(sizeof(char) *100);	
	for(i = 0; i < strlen(argv[1]); i++){
		delims[i] = argv[1][i];
	}

	i = 0;
	while(argv[1][i] != '\0'){
		delimTwo[i] = argv[1][i];
		i++;
	}

	printf("%s \n", delims);
	test(delimTwo);
	free(delimTwo);
	free(delims);

	return 0;
}
