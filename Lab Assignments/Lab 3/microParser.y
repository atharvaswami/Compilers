%{
	#include <string.h> 
	#include <stdlib.h> 
	#include <stdio.h> 

	int yylex();

	int decl_var = 0, scope = -1, block_count = 0;
	char *var_type, *scope_names[500];

	struct VarEntry{
		char *id, *type;
	};

	struct StrEntry{
		char *id, *value;
	};

	struct SymbolTable{
		char first;
		int str_count, var_count;
		struct StrEntry strings[500];
		struct VarEntry variables[500];
	};

	struct SymbolTable symbol_table[500];
	void yyerror(const char *err);
%}

%token INTLITERAL FLOATLITERAL PROGRAM _BEGIN END FUNCTION READ WRITE IF ELSE FI FOR ROF RETURN INT VOID STRING FLOAT STRINGLITERAL IDENTIFIER CONTINUE BREAK

%type <s_entry> string_decl
%type <s> id str
%type <v_entry> var_decl param_decl id_list id_tail
%type <v> var_type

%union{
	struct SymbolTable *s_table;
	struct StrEntry *s_entry;
	struct VarEntry *v_entry;
	char *s;
	char *v;
}

%%

program:	PROGRAM id _BEGIN {
				scope += 1; 
				scope_names[scope] = "GLOBAL";
				symbol_table[scope].first = 'c';  
				symbol_table[scope].str_count = 0;
				symbol_table[scope].var_count = 0; 
			}
			pgm_body END {
				for (int i = 0; i <= scope; i++) {
					if (i > 0) {
						printf("\n");
					}   
					printf("Symbol table %s", scope_names[i]);    
					if (!strcmp(scope_names[i], "BLOCK ")) {
						block_count += 1;
						printf("%d", block_count);    
					}
					printf("\n");    
					if (symbol_table[i].first == 's') {
						for (int j = 0; j < symbol_table[i].str_count; j++) {
							printf("name %s type %s value %s\n", symbol_table[i].strings[j].id, "STRING", symbol_table[i].strings[j].value);    
						}
						for (int k = 0; k < symbol_table[i].var_count; k++) {
							printf("name %s type %s\n", symbol_table[i].variables[k].id, symbol_table[i].variables[k].type);    
						}
					}
					else {
						for (int j = 0; j < symbol_table[i].var_count; j++) {
							printf("name %s type %s\n", symbol_table[i].variables[j].id, symbol_table[i].variables[j].type);    
						}
						for (int k = 0; k < symbol_table[i].str_count; k++) {
							printf("name %s type %s value %s\n", symbol_table[i].strings[k].id, "STRING", symbol_table[i].strings[k].value);    
						}
					}
				}
			};
id:			IDENTIFIER {};
pgm_body:	decl func_declarations;
decl:		string_decl decl | var_decl decl | ;
string_decl:	STRING id ':''=' str ';' {
					if (symbol_table[scope].first == 'c') {
						symbol_table[scope].first = 's';
					}
					for (int i = 0; i < symbol_table[scope].str_count; i++) {
						if (!strcmp(symbol_table[scope].strings[i].id, $2)) {
							printf("DECLARATION ERROR %s\n", $2);
							return 0;
						}
					}
					for (int i = 0; i < symbol_table[scope].var_count; i++) {
						if (!strcmp(symbol_table[scope].variables[i].id, $2)) {
							printf("DECLARATION ERROR %s\n", $2);
							return 0;
						}
					}
					$$ = malloc(sizeof(struct StrEntry())); 
					$$->id = $2; 
					$$->value = $5; 
					symbol_table[scope].strings[symbol_table[scope].str_count] = *($$); 
					symbol_table[scope].str_count += 1; 
				};
str:		STRINGLITERAL {};
var_decl:	var_type {
				if (symbol_table[scope].first == 'c') {
					symbol_table[scope].first = 'v';
				}
				decl_var = 1;   
				var_type = $1;
			} 
			id_list ';' {};
var_type:	FLOAT {} | INT {};
any_type:	var_type | VOID;
id_list:	id { 
				if (decl_var == 1) {
					for (int i = 0; i < symbol_table[scope].str_count; i++) {
						if (!strcmp(symbol_table[scope].strings[i].id, $1)) {
							printf("DECLARATION ERROR %s\n", $1);
							return 0;
						}
					}
					for (int i = 0; i < symbol_table[scope].var_count; i++) {
						if (!strcmp(symbol_table[scope].variables[i].id, $1)) {
							printf("DECLARATION ERROR %s\n", $1);
							return 0;
						}
					}
					$<v_entry>$ = malloc(sizeof(struct VarEntry())); 
					$<v_entry>$->id = $1; 
					$<v_entry>$->type = var_type;
					symbol_table[scope].variables[symbol_table[scope].var_count] = *($<v_entry>$); 
					symbol_table[scope].var_count += 1;
				}
			} 
			id_tail {};
id_tail:	',' id {
				if (decl_var == 1) {
					for (int i = 0; i < symbol_table[scope].str_count; i++) {
						if (!strcmp(symbol_table[scope].strings[i].id, $2)) {
							printf("DECLARATION ERROR %s\n", $2);
							return 0;
						}
					}
					for (int i = 0; i < symbol_table[scope].var_count; i++) {
						if (!strcmp(symbol_table[scope].variables[i].id, $2)) {
							printf("DECLARATION ERROR %s\n", $2);
							return 0;
						}
					}    
					$<v_entry>$ = malloc(sizeof(struct VarEntry())); 
					$<v_entry>$->id = $2; 
					$<v_entry>$->type = var_type;
					symbol_table[scope].variables[symbol_table[scope].var_count] = *($<v_entry>$); 
					symbol_table[scope].var_count += 1;
				}
			}
			id_tail {} | {
				decl_var = 0;
			};
param_decl_list:	param_decl param_decl_tail | ;
param_decl:	var_type id {
				if (symbol_table[scope].first == 'c') {
					symbol_table[scope].first = 'v';
				}
				for (int i = 0; i < symbol_table[scope].str_count; i++) {
					if (!strcmp(symbol_table[scope].strings[i].id, $2)) {
						printf("DECLARATION ERROR %s\n", $2);
						return 0;
					}
				}
				for (int i = 0; i < symbol_table[scope].var_count; i++) {
					if (!strcmp(symbol_table[scope].variables[i].id, $2)) {
						printf("DECLARATION ERROR %s\n", $2);
						return 0;
					}
				}    
				$$ = malloc(sizeof(struct VarEntry())); 
				$$->id = $2; 
				$$->type = $1; 
				symbol_table[scope].variables[symbol_table[scope].var_count] = *($$); 
				symbol_table[scope].var_count += 1;
			}
			;
param_decl_tail:	',' param_decl param_decl_tail | ;
func_declarations:	func_decl func_declarations | ;
func_decl:	FUNCTION any_type id {
				scope += 1; 
				scope_names[scope] = $3;
				symbol_table[scope].first = 'c';
				symbol_table[scope].str_count = 0;
				symbol_table[scope].var_count = 0;
			}
			'(' param_decl_list ')' _BEGIN func_body END;
func_body:	decl stmt_list;
stmt_list:	stmt stmt_list | ;
stmt:		base_stmt | if_stmt | for_stmt;
base_stmt:	assign_stmt | read_stmt | write_stmt | return_stmt;
assign_stmt:	assign_expr ';';
assign_expr:	id ':''=' expr;
read_stmt:	READ '(' id_list ')'';';
write_stmt:	WRITE '(' id_list ')'';';
return_stmt:	RETURN expr ';';
expr:		expr_prefix factor;
expr_prefix:	expr_prefix factor addop | ;
factor:		factor_prefix postfix_expr;
factor_prefix:	factor_prefix postfix_expr mulop | ;
postfix_expr:	primary | call_expr;
call_expr:	id '(' expr_list ')';
expr_list:	expr expr_list_tail | ;
expr_list_tail:	',' expr expr_list_tail | ;
primary:	'(' expr ')' | id | INTLITERAL | FLOATLITERAL;
addop:		'+'|'-';
mulop:		'*'|'/';
if_stmt:	IF {
				scope += 1; 
				scope_names[scope] = "BLOCK ";
				symbol_table[scope].first = 'c';
				symbol_table[scope].str_count = 0;
				symbol_table[scope].var_count = 0;
			}
			'(' cond ')' decl stmt_list else_part FI;
else_part:	ELSE {
				scope += 1; 
				scope_names[scope] = "BLOCK ";
				symbol_table[scope].first = 'c';
				symbol_table[scope].str_count = 0;
				symbol_table[scope].var_count = 0;
			}
			decl stmt_list | ;
cond:		expr compop expr;
compop:		'<'|'>'|'='|'!''='|'<''='|'>''=';
init_stmt:	assign_expr | ;
incr_stmt:	assign_expr | ;
for_stmt:	FOR {
				scope += 1; 
				scope_names[scope] = "BLOCK ";
				symbol_table[scope].first = 'c';
				symbol_table[scope].str_count = 0;
				symbol_table[scope].var_count = 0; 
			}
			'(' init_stmt ';' cond ';' incr_stmt ')' decl aug_stmt_list ROF;
aug_stmt_list:  aug_stmt aug_stmt_list | ;
aug_stmt:   base_stmt | aug_if_stmt | for_stmt | CONTINUE ';' | BREAK ';';
aug_if_stmt: IF '(' cond ')' decl aug_stmt_list aug_else_part FI;
aug_else_part:  ELSE decl aug_stmt_list | ;

%%