#include "microParser.h"
#include <stdio.h>

extern FILE* yyin;

void yyerror(const char *msg) {
	printf("Not accepted\n");
}

int main(int argc, char* argv[]) {
	if(argc > 1) {
		FILE *file_pointer = fopen(argv[1], "r");
		if(file_pointer)
			yyin = file_pointer;
	}

	int yylex();
	int yyparse();
	
    if (yyparse() == 0)
        printf("Accepted\n");

	return 0;
}