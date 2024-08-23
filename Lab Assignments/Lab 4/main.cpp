// Acknowledgement: https://github.com/aswanthpp/Compiler-Design

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include "assemblyCode.hpp"
#include "ast.hpp"
#include "codeObject.hpp"
#include "parser.h"
#include "symbolTableStack.hpp"

using namespace std;

extern AssemblyCode *assembly_code;
extern CodeObject *threeAC;
extern SymbolTableStack *tableStack;
extern FILE *yyin;

int yylex();
int yyparse();

void yyerror(char const *msg) {
    
};

int main(int argc, char* argv[]) {
    int retval;

    if (argc < 2) {
        printf("usage: ./compiler <filename> \n");
    }

    else {

        if (!(yyin = fopen(argv[1], "r"))) {
            printf("Error while opening the file.\n"); 
        }

        else {
            yyin = fopen(argv[1], "r");
			retval = yyparse();
            fclose(yyin);
            assembly_code->generateCode(threeAC, tableStack);
            assembly_code->print();
            cout << "sys halt" << endl;
        }
    }
    
    return 0;
}