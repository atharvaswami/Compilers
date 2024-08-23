%{
	#include "microParser.h"
%}

IDENTIFIER [a-zA-Z_][a-zA-Z0-9_]*
INTLITERAL [0-9]+
FLOATLITERAL [0-9]+[.][0-9]*
STRINGLITERAL ["][^"]+["]
COMMENT (#|--).*
OPERATOR [:]|[+]|[-]|[*]|[/]|[=]|[!]|[<]|[>]|[(]|[)]|[;]|[,]
SPACE [ \n\t\r]+

%%

{COMMENT} ;

{SPACE} ;

{INTLITERAL} return INTLITERAL;

{FLOATLITERAL} return FLOATLITERAL;

BEGIN   return _BEGIN;

PROGRAM return PROGRAM;

END return END;

FUNCTION    return FUNCTION;

READ    return READ;

WRITE   return WRITE;

IF  return IF;

ELSE    return ELSE;

FI  return FI;

FOR return FOR;

ROF return ROF;

RETURN  return RETURN;

INT return INT;

VOID    return VOID;

STRING  return STRING;

FLOAT   return FLOAT;

CONTINUE  return CONTINUE;

BREAK   return BREAK;

{STRINGLITERAL} return STRINGLITERAL;

{IDENTIFIER} return IDENTIFIER;

{OPERATOR} return *yytext;

%%

int yywrap() {
  	return 1;
}