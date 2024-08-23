/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

#ifndef YY_YY_PARSER_H_INCLUDED
# define YY_YY_PARSER_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    _PROGRAM = 258,
    _BEGIN = 259,
    _VOID = 260,
    _INT = 261,
    _FLOAT = 262,
    _STRING = 263,
    _READ = 264,
    _WRITE = 265,
    _FUNCTION = 266,
    _RETURN = 267,
    _IF = 268,
    _ELSE = 269,
    _FI = 270,
    _FOR = 271,
    _ROF = 272,
    _END = 273,
    ASSIGN_OP = 274,
    ADD_OP = 275,
    SUB_OP = 276,
    MUL_OP = 277,
    DIV_OP = 278,
    EQ_OP = 279,
    NEQ_OP = 280,
    LT_OP = 281,
    LTE_OP = 282,
    GT_OP = 283,
    GTE_OP = 284,
    OPEN_PAR = 285,
    CLOSED_PAR = 286,
    SEMICOLON = 287,
    COMMA = 288,
    _CONTINUE = 289,
    _BREAK = 290,
    IDENTIFIER = 291,
    INTLITERAL = 292,
    FLOATLITERAL = 293,
    STRINGLITERAL = 294
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 33 "parser.y"

    int intval;
    float floatval;
    string* stringval;
    vector<string*> *stringlist;
    ASTNode *astnode;
    vector<ASTNode*> *astlist;

#line 106 "parser.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_PARSER_H_INCLUDED  */
