%{
	#include <iostream>
	#include <string>
	#include <vector>
    #include "headers/ast.hpp"
    #include "headers/codeObject.hpp"
    #include "headers/assemblyCode.hpp"
	#include "headers/symbolTableStack.hpp"
    
    using namespace std;

	int yylex();
	void yyerror(char const *s);
	SymbolTableStack *tableStack = new SymbolTableStack();
    CodeObject *threeAC = new CodeObject(tableStack);
    AssemblyCode *assembly_code = new AssemblyCode();
%}

%token _PROGRAM _BEGIN _VOID _INT _FLOAT _STRING _READ _WRITE _FUNCTION _RETURN _IF _ELSE _FI _FOR _ROF _END ASSIGN_OP ADD_OP SUB_OP MUL_OP DIV_OP EQ_OP NEQ_OP LT_OP LTE_OP GT_OP GTE_OP OPEN_PAR CLOSED_PAR SEMICOLON COMMA _CONTINUE _BREAK
%token <stringval> IDENTIFIER
%token <intval> INTLITERAL
%token <floatval> FLOATLITERAL
%token <stringval> STRINGLITERAL

%type <intval> var_type
%type <stringval> str id
%type <stringlist> id_list id_tail
%type <astnode> mulop addop primary postfix_expr factor_prefix factor expr_prefix expr return_stmt call_expr compop for_assign_expr incr_stmt
%type <astlist> expr_list expr_list_tail

%union {
    int intval;
    float floatval;
    std::string* stringval;
    std::vector<std::string*> *stringlist;
    ASTNode *astnode;
    std::vector<ASTNode*> *astlist;
}

%%

program:			_PROGRAM id _BEGIN { tableStack->addNewTable("GLOBAL"); } pgm_body _END { tableStack->removeTable(); };
id:					IDENTIFIER;
pgm_body:			decl func_declarations;
decl:				string_decl decl | var_decl decl | ;
string_decl:		_STRING id ASSIGN_OP str SEMICOLON { tableStack->insertSymbol(*($2), "STRING", *($4)); };
str:				STRINGLITERAL;
var_decl:			var_type id_list SEMICOLON {
						std::string str_type = "";
						std::vector<std::string*> list = *$2;
						for (int i = list.size(); i != 0; --i) {
							if ($1 == _FLOAT) {
								tableStack->insertSymbol(*(list[i-1]), "FLOAT");
							}
							else if ($1 == _INT) {
								tableStack->insertSymbol(*(list[i-1]), "INT");
							}
						}
					};
var_type:			_FLOAT { $$ = _FLOAT; } | _INT { $$ = _INT; };
any_type:			var_type | _VOID;
id_list:			id id_tail {
						$$ = $2;
						$$->push_back($1);
					};
id_tail:			COMMA id id_tail {
						$$ = $3;
						$$->push_back($2);
					} | {
						std::vector<std::string*>* temp = new std::vector<std::string*>;
						$$ = temp;
					};
param_decl_list:	param_decl param_decl_tail |;
param_decl:			var_type id;
param_decl_tail:	COMMA param_decl param_decl_tail | ;
func_declarations:	func_decl func_declarations | ;
func_decl:			_FUNCTION any_type id { 
                        tableStack->addNewTable(*($3)); 
                        threeAC->threeAC.push_back(new CodeLine(threeAC->symbolTableStack->table_stack.top()->scope, "LABEL", *($3)));
                        threeAC->threeAC.push_back(new CodeLine(threeAC->symbolTableStack->table_stack.top()->scope, "LINK", ""));
                    } OPEN_PAR param_decl_list CLOSED_PAR _BEGIN func_body _END { tableStack->removeTable(); };
func_body:			decl stmt_list;
stmt_list:			stmt stmt_list | ;
stmt:				base_stmt | if_stmt | for_stmt | _CONTINUE SEMICOLON | _BREAK SEMICOLON {
                        int x = threeAC->lbList.front();
                        x++;
                        threeAC->threeAC.push_back(new CodeLine(threeAC->symbolTableStack->table_stack.top()->scope, "JUMP", "LABEL"+std::to_string(x)));
                    };
base_stmt:			assign_stmt | read_stmt | write_stmt | return_stmt;
assign_stmt:		assign_expr SEMICOLON;
assign_expr:		id ASSIGN_OP expr {
                        ASTNode * node = new ASTNode_Assign(tableStack->findEntry(*$1));
                        node->right = $3;
                        node->generateCode(threeAC);
                    };
read_stmt:			_READ OPEN_PAR id_list CLOSED_PAR SEMICOLON {
                        std::vector<std::string*> list = *($3);
                        for (int i = list.size(); i != 0; --i) {
                            std::string name = *(list[i-1]);
                            std::string type = tableStack->findType(name);
                            threeAC->addRead(name, type);
                        }
                    };
write_stmt:			_WRITE OPEN_PAR id_list CLOSED_PAR SEMICOLON {
                        std::vector<std::string*> list = *($3);
                        for (int i = list.size(); i != 0; --i) {
                            std::string name = *(list[i-1]);
                            std::string type = tableStack->findType(name);
                            threeAC->addWrite(name, type);
                        }
                    };
return_stmt:		_RETURN expr SEMICOLON {};
expr:				expr_prefix factor {
                        if ($1 == nullptr)
                            $$ = $2;
                        else {
                            $1->right = $2;
                            $$ = $1;
                        }
                    };
expr_prefix:		expr_prefix factor addop {
                        if ($1 == nullptr)
                            $3->left = $2;
                        else {
                            $1->right = $2;
                            $3->left = $1;
                        }
                        $$ = $3;
                    } | { $$ = nullptr; };
factor:				factor_prefix postfix_expr{
                        if ($1 == nullptr)
                            $$ = $2;
                        else
                        {
                            $1->right = $2;
                            $$ = $1;
                        }
                    };
factor_prefix:		factor_prefix postfix_expr mulop {
                        if ($1 == nullptr)
                            $3->left = $2;
                        else
                        {
                            $1->right = $2;
                            $3->left = $1;
                        }
                        $$ = $3;
                    } | { $$ = nullptr; };
postfix_expr:		primary { $$ = $1; } | call_expr { $$ = $1; };
call_expr:			id OPEN_PAR expr_list CLOSED_PAR {};
expr_list:			expr expr_list_tail {
                        $$ = $2;
                        $$->push_back($1);
                    } | {
                        std::vector<ASTNode *> *node = new std::vector<ASTNode*>;
                        $$ = node;
                    };
expr_list_tail:		COMMA expr expr_list_tail {
                        $$ = $3;
                        $$->push_back($2);
                    } | {
                        std::vector<ASTNode *> *node = new std::vector<ASTNode*>;
                        $$ = node;
                    };
primary:			OPEN_PAR expr CLOSED_PAR {$$ = $2;} | id {
                        $$ = new ASTNode_ID(tableStack->findEntry(*$1));
                    } | INTLITERAL {
                        $$ = new ASTNode_INT($1);
                    } | FLOATLITERAL {
                        $$ = new ASTNode_FLOAT($1);
                    };
addop:				ADD_OP { $$ = new ASTNode_Expr('+'); } | SUB_OP { $$ = new ASTNode_Expr('-'); };
mulop:				MUL_OP { $$ = new ASTNode_Expr('*'); } | DIV_OP { $$ = new ASTNode_Expr('/'); };
if_stmt:			_IF { tableStack->addNewTable(); } OPEN_PAR cond CLOSED_PAR decl stmt_list {
                        threeAC->lb += 1;
                        threeAC->lbList.push_front(threeAC->lb);
                        threeAC->threeAC.push_back(new CodeLine(threeAC->symbolTableStack->table_stack.top()->scope, "JUMP", "LABEL"+std::to_string(threeAC->lb)));

                        int x = threeAC->lbList.back();
                        threeAC->lbList.pop_back();
                        threeAC->threeAC.push_back(new CodeLine(threeAC->symbolTableStack->table_stack.top()->scope, "LABEL", "LABEL"+std::to_string(x)));
                        tableStack->removeTable(); 

                    } else_part _FI {
                        int x = threeAC->lbList.front();
                        threeAC->lbList.pop_front();
                        threeAC->threeAC.push_back(new CodeLine(threeAC->symbolTableStack->table_stack.top()->scope, "LABEL", "LABEL"+std::to_string(x)));
                    };
else_part:			_ELSE { tableStack->addNewTable(); } decl stmt_list { tableStack->removeTable(); } | ;
cond:				expr compop expr {
                        $2->left = $1;
                        $2->right = $3;
                        $2->generateCode(threeAC);   
                    };
compop:				LT_OP { $$ = new ASTNode_Cond("<"); } | GT_OP { $$ = new ASTNode_Cond(">"); } | EQ_OP { $$ = new ASTNode_Cond("="); } | NEQ_OP { $$ = new ASTNode_Cond("!="); } | LTE_OP { $$ = new ASTNode_Cond("<="); } | GTE_OP { $$ = new ASTNode_Cond(">="); };
init_stmt:			assign_expr | ;
incr_stmt:			for_assign_expr  { $$ = $1; } | { $$ = nullptr; };
for_assign_expr:    id ASSIGN_OP expr {
                        ASTNode * node = new ASTNode_Assign(tableStack->findEntry(*$1));
                        node->right = $3;
                        $$ = node;
                    };
for_stmt:			_FOR { tableStack->addNewTable(""); } OPEN_PAR init_stmt SEMICOLON {
                        threeAC->lb += 1;
                        threeAC->lbList.push_front(threeAC->lb);
                        threeAC->threeAC.push_back(new CodeLine(threeAC->symbolTableStack->table_stack.top()->scope, "LABEL", "LABEL"+std::to_string(threeAC->lb)));
                    } cond REST; 
REST:               SEMICOLON incr_stmt CLOSED_PAR decl stmt_list _ROF{
                        if($2!=nullptr) {
                            ($2)->generateCode(threeAC);
                            int x = threeAC->lbList.front();
                            threeAC->lbList.pop_front();
                            threeAC->threeAC.push_back(new CodeLine(threeAC->symbolTableStack->table_stack.top()->scope, "JUMP", "LABEL"+std::to_string(x)));

                            x = threeAC->lbList.back();
                            threeAC->lbList.pop_back();
                            threeAC->threeAC.push_back(new CodeLine(threeAC->symbolTableStack->table_stack.top()->scope, "LABEL", "LABEL"+std::to_string(x)));
                            tableStack->removeTable();
                        }
                        else {
                            int x = threeAC->lbList.front();
                            threeAC->lbList.pop_front();
                            threeAC->threeAC.push_back(new CodeLine(threeAC->symbolTableStack->table_stack.top()->scope, "JUMP", "LABEL"+std::to_string(x)));

                            x = threeAC->lbList.back();
                            threeAC->lbList.pop_back();
                            threeAC->threeAC.push_back(new CodeLine(threeAC->symbolTableStack->table_stack.top()->scope, "LABEL", "LABEL"+std::to_string(x)));
                            tableStack->removeTable();
                        }
                    };

%%