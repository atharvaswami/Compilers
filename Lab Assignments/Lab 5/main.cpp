#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include "headers/ast.hpp"
#include "headers/codeObject.hpp"
#include "headers/assemblyCode.hpp"
#include "headers/symbolTableStack.hpp"
#include "parser.h"

using namespace std;

extern AssemblyCode *assembly_code;
extern CodeObject *threeAC;
extern FILE *yyin;

int yylex ();
int yyparse ();

void yyerror (char const *msg) {
};

int main (int argc, char* argv[]) {
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

            cout << "push" << endl;
            cout << "push r0" << endl;
            cout << "push r1" << endl;
            cout << "push r2" << endl;
            cout << "push r3" << endl;
            cout << "jsr main" << endl;
            cout << "sys halt" << endl;
            assembly_code->generateCode(threeAC, threeAC->symbolTableStack->tables);
            assembly_code->print();
            cout << "end" << endl;
        }
    }
    
    return 0;
}