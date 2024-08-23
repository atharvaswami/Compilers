%{
    #include<iostream>
    #include<string>
    #include<vector>
    #include<utility>
    #include "header.hpp"

    int for_count = 0;
    int back_count = 0;
    int front_count = 0;
	extern "C" int yylex();
    extern char* yytext;
	void yyerror(char const *s);
	SymbolTableStack *stackTable = new SymbolTableStack();
    CodeObject *AC3 = new CodeObject(stackTable);
    AssemblyCode *assembly_code = new AssemblyCode();
%}

%token _PROGRAM _BEGIN _VOID _INT _FLOAT _STRING _READ _WRITE _FUNCTION _RETURN _IF _ELSE _FI _FOR _ROF _END ASSIGN_OP ADD_OP SUB_OP MUL_OP DIV_OP EQ_OP NEQ_OP LT_OP LTE_OP GT_OP GTE_OP OPEN_PAR CLOSED_PAR SEMICOLON COMMA _CONTINUE _BREAK IDENTIFIER INTLITERAL FLOATLITERAL STRINGLITERAL

%type <stringlist> id_list id_tail
%type <floatval> _FLOAT FLOATLITERAL
%type <astlist> expr_list expr_list_tail
%type <stringval> str id IDENTIFIER STRINGLITERAL
%type <intval> var_type _INT any_type _VOID INTLITERAL
%type <astnode> mulop addop primary postfix_expr factor_prefix factor expr_prefix expr return_stmt call_expr compop for_assign_expr incr_stmt

%union {
    int intval;
    float floatval;
    std::string* stringval;
    std::vector<std::string*> *stringlist;
    ASTNode *astnode;
    std::vector<ASTNode*> *astlist;
}

%%

program:			_PROGRAM id _BEGIN { stackTable->addNewTable("GLOBAL"); } pgm_body _END { stackTable->removeTable(); };
id:					IDENTIFIER;
pgm_body:			decl func_declarations;
decl:				string_decl decl | var_decl decl | ;
string_decl:		_STRING id ASSIGN_OP str SEMICOLON { stackTable->insertSymbol(*($2), "STRING", *($4)); };
str:				STRINGLITERAL;
var_decl:			var_type id_list SEMICOLON {
						std::vector<std::string*> list = *$2;
						for (int i = list.size() - 1; i >= 0; i--) {
							if ($1 == _FLOAT) {
								stackTable->insertSymbol(*list[i], "FLOAT");
							}
							else if ($1 == _INT) {
								stackTable->insertSymbol(*list[i], "INT");
							}
						}
					};
var_type:			_FLOAT { $$ = _FLOAT; } | _INT { $$ = _INT; };
any_type:			var_type { $$ = $1; } | _VOID { $$ = $1; };
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
param_decl:			var_type id {
						if ($1 == _FLOAT)
							stackTable->insertSymbol(*($2), "FLOAT", 1);
						else if ($1 == _INT)
							stackTable->insertSymbol(*($2), "INT", 1);
					};
param_decl_tail:	COMMA param_decl param_decl_tail | ;
func_declarations:	func_decl func_declarations | ;
func_decl:			_FUNCTION any_type id { 
                        stackTable->addNewTable(*($3)); 
                        AC3->AC3.push_back(new CodeLine(AC3->symbolTableStack->table_stack.top()->scope, "LABEL", *($3)));
                        AC3->AC3.push_back(new CodeLine(AC3->symbolTableStack->table_stack.top()->scope, "LINK", ""));
                    } OPEN_PAR param_decl_list CLOSED_PAR _BEGIN func_body _END { stackTable->removeTable(); };
func_body:			decl stmt_list;
stmt_list:			stmt stmt_list | ;
stmt:				base_stmt | if_stmt | for_stmt;
base_stmt:			assign_stmt | read_stmt | write_stmt | return_stmt;
assign_stmt:		assign_expr SEMICOLON;
assign_expr:		id ASSIGN_OP expr {
                        ASTNode * node = new ASTNode_Assign(stackTable->findEntry(*$1));
                        node->right = $3;
                        node->generateCode(AC3);
                    };
read_stmt:			_READ OPEN_PAR id_list CLOSED_PAR SEMICOLON {
                        std::vector<std::string*> list = *($3);
                        for (int i = list.size() - 1; i >= 0; i--) {
                            std::string name = *(list[i]);
                            std::string type = stackTable->findType(name);
                            AC3->addRead(name, type);
                        }
                    };
write_stmt:			_WRITE OPEN_PAR id_list CLOSED_PAR SEMICOLON {
                        std::string type;
                        std::vector<std::string*> list = *($3);
                        for (int i = list.size() - 1; i >= 0; i--) {
                            std::string name = *(list[i]);
                            if (name == "newline")
                                type = "STRING";
                            else
                                type = stackTable->findType(name);
                            AC3->addWrite(name, type);
                        }
                    };
return_stmt:		_RETURN expr SEMICOLON {
                        std::string a = ($2)->generateCode(AC3);
                        AC3->AC3.push_back(new CodeLine(AC3->symbolTableStack->table_stack.top()->scope, "RET", a));
                    };
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
factor:				factor_prefix postfix_expr {
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
                        else {
                            $1->right = $2;
                            $3->left = $1;
                        }
                        $$ = $3;
                    } | { $$ = nullptr; };
postfix_expr:		primary { $$ = $1; } | call_expr { $$ = $1; };
call_expr:			id OPEN_PAR expr_list CLOSED_PAR {
                        $$ = new ASTNode_CallExpr(*($1), $3);
                    };
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
                        $$ = new ASTNode_ID(stackTable->findEntry(*$1));
                    } | INTLITERAL {
                        $$ = new ASTNode_INT($1);
                    } | FLOATLITERAL {
                        $$ = new ASTNode_FLOAT($1);
                    };
addop:				ADD_OP { $$ = new ASTNode_Expr('+'); } | SUB_OP { $$ = new ASTNode_Expr('-'); };
mulop:				MUL_OP { $$ = new ASTNode_Expr('*'); } | DIV_OP { $$ = new ASTNode_Expr('/'); };
if_stmt:			_IF { stackTable->addNewTable(); } OPEN_PAR cond { back_count++; } CLOSED_PAR decl aug_stmt_list {
                        AC3->lb++;
                        AC3->lbList.push_front(AC3->lb);
                        front_count++;
                        AC3->AC3.push_back(new CodeLine(AC3->symbolTableStack->table_stack.top()->scope, "JUMP", "LABEL"+std::to_string(AC3->lb)));

                        int x = AC3->lbList.back();
                        AC3->lbList.pop_back();
                        back_count--;
                        AC3->AC3.push_back(new CodeLine(AC3->symbolTableStack->table_stack.top()->scope, "LABEL", "LABEL"+std::to_string(x)));
                        stackTable->removeTable(); 
                    } else_part _FI {
                        int x = AC3->lbList.front();
                        AC3->lbList.pop_front();
                        front_count--;
                        AC3->AC3.push_back(new CodeLine(AC3->symbolTableStack->table_stack.top()->scope, "LABEL", "LABEL"+std::to_string(x)));
                    };
else_part:			_ELSE { stackTable->addNewTable(); } decl aug_stmt_list { stackTable->removeTable(); } | ;
cond:				expr compop expr {
                        $2->left = $1;
                        $2->right = $3;
                        $2->generateCode(AC3);   
                    };
compop:				LT_OP { $$ = new ASTNode_Cond("<"); } | GT_OP { $$ = new ASTNode_Cond(">"); } | EQ_OP { $$ = new ASTNode_Cond("="); } | NEQ_OP { $$ = new ASTNode_Cond("!="); } | LTE_OP { $$ = new ASTNode_Cond("<="); } | GTE_OP { $$ = new ASTNode_Cond(">="); };
init_stmt:			assign_expr | ;
incr_stmt:			for_assign_expr  { $$ = $1; } | { $$ = nullptr; };
for_assign_expr:    id ASSIGN_OP expr {
                        ASTNode * node = new ASTNode_Assign(stackTable->findEntry(*$1));
                        node->right = $3;
                        $$ = node;
                    };
for_stmt:			_FOR { stackTable->addNewTable(); } OPEN_PAR init_stmt SEMICOLON {
                        AC3->lb++;
                        AC3->lbList.push_front(AC3->lb);
                        for_count--;
                        AC3->lbListFor.push_back(for_count);
                        AC3->AC3.push_back(new CodeLine(AC3->symbolTableStack->table_stack.top()->scope, "LABEL", "LABEL"+std::to_string(AC3->lb)));
                    } cond REST; 
REST:               SEMICOLON incr_stmt CLOSED_PAR decl aug_stmt_list {
                        int x = AC3->lbListFor.back();
                        AC3->lbListFor.pop_back();
                        AC3->AC3.push_back(new CodeLine(AC3->symbolTableStack->table_stack.top()->scope, "LABEL", "LABEL"+std::to_string(x)));
                    } _ROF{
                        if($2!=nullptr) {
                            ($2)->generateCode(AC3);
                            int x = AC3->lbList.front();
                            AC3->lbList.pop_front();
                            AC3->AC3.push_back(new CodeLine(AC3->symbolTableStack->table_stack.top()->scope, "JUMP", "LABEL"+std::to_string(x)));

                            x = AC3->lbList.back();
                            AC3->lbList.pop_back();
                            AC3->AC3.push_back(new CodeLine(AC3->symbolTableStack->table_stack.top()->scope, "LABEL", "LABEL"+std::to_string(x)));
                            stackTable->removeTable();
                        }
                        else {
                            int x = AC3->lbList.front();
                            AC3->lbList.pop_front();
                            AC3->AC3.push_back(new CodeLine(AC3->symbolTableStack->table_stack.top()->scope, "JUMP", "LABEL"+std::to_string(x)));

                            x = AC3->lbList.back();
                            AC3->lbList.pop_back();
                            AC3->AC3.push_back(new CodeLine(AC3->symbolTableStack->table_stack.top()->scope, "LABEL", "LABEL"+std::to_string(x)));
                            stackTable->removeTable();
                        }
                    };
aug_stmt_list:  aug_stmt aug_stmt_list | ;
aug_stmt:   base_stmt | aug_if_stmt | for_stmt | _CONTINUE SEMICOLON {
                        auto it = AC3->lbList.begin();
                        for (int i = 0; i < front_count; i++) {
                            it++;
                        }
                        int x = AC3->lbListFor.back();
                        AC3->AC3.push_back(new CodeLine(AC3->symbolTableStack->table_stack.top()->scope, "JUMP", "LABEL"+std::to_string(x)));
                    } | _BREAK SEMICOLON {
                        auto it = AC3->lbList.end();
                        it--;
                        for (int i = 0; i < back_count; i++) {
                            it--;
                        }
                        int x = *it;
                        AC3->AC3.push_back(new CodeLine(AC3->symbolTableStack->table_stack.top()->scope, "JUMP", "LABEL"+std::to_string(x)));
                    };
aug_if_stmt: _IF { stackTable->addNewTable(); } OPEN_PAR cond { back_count++; } CLOSED_PAR decl aug_stmt_list {
                        AC3->lb++;
                        AC3->lbList.push_front(AC3->lb);
                        front_count++;
                        AC3->AC3.push_back(new CodeLine(AC3->symbolTableStack->table_stack.top()->scope, "JUMP", "LABEL"+std::to_string(AC3->lb)));

                        int x = AC3->lbList.back();
                        AC3->lbList.pop_back();
                        back_count--;
                        AC3->AC3.push_back(new CodeLine(AC3->symbolTableStack->table_stack.top()->scope, "LABEL", "LABEL"+std::to_string(x)));
                        stackTable->removeTable(); 

                    } aug_else_part _FI {
                        int x = AC3->lbList.front();
                        AC3->lbList.pop_front();
                        front_count--;
                        AC3->AC3.push_back(new CodeLine(AC3->symbolTableStack->table_stack.top()->scope, "LABEL", "LABEL"+std::to_string(x)));
                    };
aug_else_part:  _ELSE { stackTable->addNewTable(); } decl aug_stmt_list { stackTable->removeTable(); } | ;

%%