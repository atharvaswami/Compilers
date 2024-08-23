#ifndef HEADER_HPP
#define HEADER_HPP

#include <bits/stdc++.h>

class Entry {
public:
    std::string name, type, value, stackname;
    bool isParameter = false;

    Entry(std::string name, std::string type) {
        this->name = name;
        this->type = type;
        this->value = "";
    }
    
    Entry(std::string name, std::string type, std::string value) {
        this->name = name;
        this->type = type;
        this->value = value;
    }

    Entry(std::string name, std::string type, bool isParameter) {
        this->name = name;
        this->type = type;
        this->isParameter = isParameter;
    }

};

class SymbolTable {
public:
    std::string scope;
    int total_parameters = 0, total_non_parameters = 0;
    std::unordered_map<std::string, Entry *> symbols;
    std::vector<Entry *> ordered_symbols;

    SymbolTable(std::string scope) {
        this->scope = scope;
    }

    int linkSize() {
        return total_non_parameters;
    }

    Entry* findEntry(std::string name) {
        return symbols[name];
    }

    void addEntry(std::string name, std::string type) {
        total_non_parameters++;
        Entry* variable = new Entry(name, type);
        variable->stackname = "$-" + std::to_string(total_non_parameters);
        ordered_symbols.push_back(variable);
        symbols[name] = variable;
        std::cout << "var " << name << std::endl;
    }

    void addEntry(std::string name, std::string type, std::string value) {
        total_non_parameters++;
        Entry* variable = new Entry(name, type, value);
        variable->stackname = value;
        ordered_symbols.push_back(variable);
        symbols[name] = variable;
        if(value ==""){
            std::cout << "var " << name << std::endl;
        }
        else{
            std::cout << "str " << name << " " << value << std::endl;
        }
    }

    void addEntry(std::string name, std::string type, bool isParameter) {
        total_parameters++;
        Entry* variable = new Entry(name, type, isParameter);
        variable->stackname = "$" + std::to_string(total_parameters+1);
        ordered_symbols.push_back(variable);
        symbols[name] = variable;
        std::cout<<"var "<<name<<std::endl;
    }

    bool ifExists(std::string name) {
        if (symbols.find(name) == symbols.end())
            return false;
        else
            return true;
    }

    void printTable() {
        std::cout << "Symbol table " << scope << std::endl;

        for (auto it = ordered_symbols.begin(); it != ordered_symbols.end(); ++it) {
            std::cout << "name " << (*it)->name << " type " << (*it)->type;    
            if ((*it)->value != "")
                std::cout << " value " << (*it)->value;
            std::cout << std::endl;
        }
    }
};

class SymbolTableStack {
public:
    int block_number = 1;
    std::string error_variable = "";
    std::vector<SymbolTable *> tables;
    std::stack<SymbolTable *> table_stack;

    void addNewTable() {
        SymbolTable *temp = new SymbolTable("$BLOCK " + std::to_string(block_number));
        table_stack.push(temp);
        tables.push_back(temp);
        block_number += 1;
    }

    void addNewTable(std::string name) {
        SymbolTable *temp = new SymbolTable(name);
        table_stack.push(temp);
        tables.push_back(temp);
    }

    void removeTable() {
        table_stack.pop();
    }

    void insertSymbol(std::string name, std::string type) {
        SymbolTable *table = table_stack.top();

        if (table->ifExists(name) && error_variable == "")
            error_variable = name;
        else
            table->addEntry(name, type);
    }

    void insertSymbol(std::string name, std::string type, std::string value) {
        SymbolTable *table = table_stack.top();

        if (table->ifExists(name) && error_variable == "")
            error_variable = name;
        else
            table->addEntry(name, type, value);
    }

    void insertSymbol(std::string name, std::string type, bool isParameter) {
        SymbolTable *table = table_stack.top();
        
        if (table->ifExists(name) && error_variable == "")
            error_variable = name;
        else
            table->addEntry(name, type, isParameter);
    }

    Entry* findEntry(std::string name) {
        std::stack<SymbolTable *> temp_stack = table_stack;
        while (temp_stack.size())
        {
            if (temp_stack.top()->ifExists(name))
                return temp_stack.top()->findEntry(name);
            temp_stack.pop();
        }
        return new Entry("error", "error");
    }

    Entry* findEntry(std::string name, std::string scope) {
        for(auto table : tables) {
            if(table->scope == scope)
                return table->findEntry(name);
        }
        return new Entry("error", "error");
    }

    std::string findType(std::string name) {
        return findEntry(name)->type;
    }

    void printStack() {
        if (error_variable != "")
            std::cout << "DECLARATION ERROR " + error_variable << std::endl;
        else {
            for (int i = 0; i != tables.size(); i++) {
                tables[i]->printTable();
                if (i != tables.size() - 1)
                    std::cout << std::endl;
            }
        }
    }
};

class CodeLine {
public:
    std::string command, arg1 ,arg2, arg3;
    std::string scope;

    CodeLine(std::string scope, std::string command, std::string arg1) {
        this->scope = scope;
        this->command = command;
        this->arg1 = arg1;
        this->arg2 = "";
        this->arg3 = "";
    }

    CodeLine(std::string scope, std::string command, std::string arg1, std::string arg2) {
        this->scope = scope;
        this->command = command;
        this->arg1 = arg1;
        this->arg2 = arg2;
        this->arg3 = "";
    }

    CodeLine(std::string scope, std::string command, std::string arg1, std::string arg2, std::string arg3) {
        this->scope = scope;
        this->command = command;
        this->arg1 = arg1;
        this->arg2 = arg2;
        this->arg3 = arg3;
    }

    void print() {
        std::string print_val =  command + " " + arg1;
        if (arg2 != "")
            print_val += " " + arg2;
        if (arg3 != "")
            print_val += " " + arg3;
        std::cout << print_val << std::endl;
    }
};

class CodeObject {   
public:
    int lb = 0;
    int temp_value = 0;
    std::deque<int> lbList;
    std::deque<int> lbListFor;
    std::vector<CodeLine*> AC3;
    SymbolTableStack* symbolTableStack;
    
    CodeObject(SymbolTableStack* symbolTableStack) {
        this->symbolTableStack = symbolTableStack;
    }

    std::string getTemp() {
        return "$T" + std::to_string(++temp_value);
    }

    void addRead(std::string var_name, std::string type) {
        if (type == "INT")
            AC3.push_back(new CodeLine(symbolTableStack->table_stack.top()->scope, "READI", var_name));
        else if (type == "FLOAT")
            AC3.push_back(new CodeLine(symbolTableStack->table_stack.top()->scope, "READF", var_name));
    }

    void addWrite(std::string var_name, std::string type) {
        if (type == "INT")
            AC3.push_back(new CodeLine(symbolTableStack->table_stack.top()->scope, "WRITEI", var_name));
        else if (type == "FLOAT")
            AC3.push_back(new CodeLine(symbolTableStack->table_stack.top()->scope, "WRITEF", var_name));
        else if (type == "STRING")
            AC3.push_back(new CodeLine(symbolTableStack->table_stack.top()->scope, "WRITES", var_name));
    }

    void print() {
        for (auto s: AC3)
            s->print();
    }

};

class ASTNode {
public:
    static std::string id_type;
    std::string type;
    ASTNode *left;
    ASTNode *right;

    virtual std::string generateCode(CodeObject *code) {
        return "N/A";
    };
};

class ASTNode_ID : public ASTNode {
public:
    std::string type = "ID";
    Entry *variable;

    ASTNode_ID(Entry *variable) {
        this->variable = variable;
    }

    std::string generateCode(CodeObject *code) {
        return this->variable->name;
    }
};

class ASTNode_INT : public ASTNode {
public:
    std::string type = "INT";
    int value;

    ASTNode_INT(int value) {
        this->value = value;
    }

    std::string generateCode(CodeObject *code) {
        std::string temp = code->getTemp();
        code->AC3.push_back(new CodeLine(code->symbolTableStack->table_stack.top()->scope, "STOREI", std::to_string(this->value), temp));
        return temp;
    }
};

class ASTNode_FLOAT : public ASTNode {
public:
    std::string type = "FLOAT";
    float value;

    ASTNode_FLOAT(float value) {
        this->value = value;
    }

    std::string generateCode(CodeObject *code) {
        std::string temp = code->getTemp();
        char str[10];
        sprintf(str, "%f", value);
        code->AC3.push_back(new CodeLine(code->symbolTableStack->table_stack.top()->scope, "STOREF", str, temp));
        return temp;
    }
};

class ASTNode_Assign : public ASTNode {
public:
    std::string type = "ASSIGN";

    ASTNode_Assign(Entry* var) {
        this->left = new ASTNode_ID(var);
        ASTNode::id_type = var->type;
    }

    std::string generateCode(CodeObject *code) {
        std::string command = "";
        if (ASTNode::id_type == "INT")
            command += "STOREI";
        else if (ASTNode::id_type == "FLOAT")
            command += "STOREF";
        code->AC3.push_back(new CodeLine(code->symbolTableStack->table_stack.top()->scope, command, this->right->generateCode(code), this->left->generateCode(code)));
        return "";
    }
};

class ASTNode_Expr : public ASTNode {
public:
    std::string type = "EXPR";
    char optype;

    ASTNode_Expr(char optype) {
        this->optype = optype;
    }

    std::string generateCode(CodeObject *code) {
        std::string left = this->left->generateCode(code);
        std::string right = this->right->generateCode(code);
        std::string temp = code->getTemp();
        std::string command = "";
        if (ASTNode::id_type == "INT") {
            if (optype == '+')
                command += "ADDI";
            else if (optype == '-')
                command += "SUBI";
            else if (optype == '*')
                command += "MULI";
            else if (optype == '/')
                command += "DIVI";
        }
        else if (ASTNode::id_type == "FLOAT") {
            if (optype == '+')
                command += "ADDF";
            else if (optype == '-')
                command += "SUBF";
            else if (optype == '*')
                command += "MULF";
            else if (optype == '/')
                command += "DIVF";
        }
        code->AC3.push_back(new CodeLine(code->symbolTableStack->table_stack.top()->scope, command, left, right, temp));
        return temp;
    }
};

class ASTNode_CallExpr : public ASTNode {
public:
    std::string type = "CALLEXPR";
    std::string funct_name = "";
    std::vector<ASTNode*>* parameter_list;

    ASTNode_CallExpr(std::string func_name, std::vector<ASTNode*>* plist) {
        this->funct_name = func_name;
        this->parameter_list = plist;
    }


    std::string generateCode(CodeObject *code) {
        std::string comm = "PUSH";
        code->AC3.push_back(new CodeLine(code->symbolTableStack->table_stack.top()->scope, comm, ""));

        for(auto& node : *parameter_list) {
            std::string para = node->generateCode(code);
            code->AC3.push_back(new CodeLine(code->symbolTableStack->table_stack.top()->scope, comm, para));
        }

        code->AC3.push_back(new CodeLine(code->symbolTableStack->table_stack.top()->scope, comm + "R", ""));
        code->AC3.push_back(new CodeLine(code->symbolTableStack->table_stack.top()->scope, "JSR", funct_name));
        code->AC3.push_back(new CodeLine(code->symbolTableStack->table_stack.top()->scope, "POPR", ""));

        for(int i=0;i<(int)(*parameter_list).size();++i)
            code->AC3.push_back(new CodeLine(code->symbolTableStack->table_stack.top()->scope, "POP", ""));

        std::string temp = code->getTemp();
        code->AC3.push_back(new CodeLine(code->symbolTableStack->table_stack.top()->scope, "POP", temp));         
        return temp;
    }
};

class ASTNode_Cond : public ASTNode {
public:
    std::string comp_op;
    ASTNode_Cond(std::string comp_op) {
        this->comp_op = comp_op;
    }

    std::string generateCode(CodeObject *code) {
        std::string op, label;
        std::string arg1 = this->left->generateCode(code);
        std::string arg2 = this->right->generateCode(code);

        code->lb += 1;
        code->lbList.push_back(code->lb);
        label = "LABEL" + std::to_string(code->lb);

        if (comp_op == ">") {
            op = "LE";
        } else if (comp_op == "<") {
            op = "GE";
        } else if (comp_op == "=") {
            op = "NE";
        } else if (comp_op == "!=") {
            op = "EQ";
        } else if (comp_op == ">=") {
            op = "LT";
        } else if (comp_op == "<=") {
            op = "GT";
        }
        code->AC3.push_back(new CodeLine(code->symbolTableStack->table_stack.top()->scope, op, arg1, arg2, label));
        return "";
    }
};

class AssemblyCode {
	public:
        int reg = 0;
        std::map<std::string, std::string> tempToReg;
        std::vector<CodeLine> assembly;

        std::string getNewReg() {
            reg++;
            return "r" + std::to_string(reg%4);
        }

        std::string getWhatever(std::string temp) {
            if(temp[0] == '#') {
                temp[0] = '$';
                return temp;
            }
            if(temp[0] != '$')
                return temp;
            if(tempToReg.find(temp) != tempToReg.end())
                return tempToReg[temp];
            tempToReg[temp] = getNewReg();
            return tempToReg[temp];
        }

        std::string getRegForID(std::string ID) {
            tempToReg[ID] = getNewReg();
            return tempToReg[ID];
        }

        bool isTemp(std::string temp) {
            return temp[0] == '$';
        }

        void generateCode(CodeObject *code, std::vector<SymbolTable *> STvector) {
            auto getLower = [&](std::string s) -> std::string {
                for(char& i : s)
                    i |= ' ';
                if(s.back() == 'f')
                    s.back() = 'r';
                return s;
            };

			for(CodeLine* line : code->AC3) {
				std::string com = line->command;

				std::string farg1, farg2, farg3;

				auto frameVar = [&](std::string s, std::string scope) -> std::string {
					if(s[0] != '$') {
						bool got = 0;
						for(int i=STvector.size()-1;i>=0;--i) {
							if(STvector[i]->scope == scope)
								got = 1;
							if(got == 1 && STvector[i]->scope[0] != '$') {
								scope = STvector[i]->scope;
								break;
							}
						}

						SymbolTable* curtable;
						for(SymbolTable* table : STvector) {
							if(table->scope == scope) {
								curtable = table;
								break;
							}
						}

						int no_parameters = 0;

						for(int i=0;i<(int)curtable->ordered_symbols.size();++i) {
							if(curtable->ordered_symbols[i]->isParameter == true)
								no_parameters ++;
						}

						for(int i=0;i<(int)curtable->ordered_symbols.size();++i) {
							if(curtable->ordered_symbols[i]->name == s) {
								if(curtable->ordered_symbols[i]->isParameter == true)
									return "#" + std::to_string(5 + i + 1);
								return "#-" + std::to_string(i + 1 - no_parameters);
							}
						}
					}
					return s;
				};

				farg1 = frameVar(line->arg1, line->scope);
				farg2 = frameVar(line->arg2, line->scope);
				farg3 = frameVar(line->arg3, line->scope);

				std::string flag = "$";
				int no_parameters = 0;

				bool got = 0;
				std::string scope = line->scope;
				for(int i=STvector.size()-1;i>=0;--i) {
					if(STvector[i]->scope == scope)
						got = 1;
					if(got == 1 && STvector[i]->scope[0] != '$') {
						scope = STvector[i]->scope;
						break;
					}
				}

				SymbolTable* curtable;
				for(SymbolTable* table : STvector) {
					if(table->scope == scope) {
						curtable = table;
						break;
					}
				}
				for(int i=0;i<(int)curtable->ordered_symbols.size();++i) {
					if(curtable->ordered_symbols[i]->isParameter == true)
						no_parameters ++;
				}

				flag += std::to_string(6 + no_parameters);
				if(com == "RET") {
					if(farg1[0] == '$') {
						assembly.push_back(CodeLine(line->scope, "move", getWhatever(farg1), flag));
						assembly.push_back(CodeLine(line->scope, "unlnk", ""));
						assembly.push_back(CodeLine(line->scope, "ret", ""));
					} else {
						std::string newR = getNewReg();
						tempToReg[farg1] = newR;
						assembly.push_back(CodeLine(line->scope, "move", getWhatever(farg1), newR));
						assembly.push_back(CodeLine(line->scope, "move", newR, flag));
						assembly.push_back(CodeLine(line->scope, "unlnk", ""));
						assembly.push_back(CodeLine(line->scope, "ret", ""));
					}
				}
				if(com == "PUSH") {
					if(line->arg1 != "")
						assembly.push_back(CodeLine(line->scope, "push", getWhatever(farg1)));
					else
						assembly.push_back(CodeLine(line->scope, "push", ""));
				}
				if(com == "PUSHR") {
					assembly.push_back(CodeLine(line->scope, "push", "r0"));
					assembly.push_back(CodeLine(line->scope, "push", "r1"));
					assembly.push_back(CodeLine(line->scope, "push", "r2"));
					assembly.push_back(CodeLine(line->scope, "push", "r3"));
				}
				if(com == "LINK") {
					int cnt = 100;
					assembly.push_back(CodeLine(line->scope, "link", std::to_string(cnt)));
				}
				if(com == "JSR") {
					assembly.push_back(CodeLine(line->scope, "jsr", line->arg1));
				}
				if(com == "POP") {
					if(line->arg1 == "")
						assembly.push_back(CodeLine(line->scope, "pop", ""));
					else 
						assembly.push_back(CodeLine(line->scope, "pop", getWhatever(farg1)));
				}
				if(com == "POPR") {
					assembly.push_back(CodeLine(line->scope, "pop", "r3"));
					assembly.push_back(CodeLine(line->scope, "pop", "r2"));
					assembly.push_back(CodeLine(line->scope, "pop", "r1"));
					assembly.push_back(CodeLine(line->scope, "pop", "r0"));
				}
				if(com == "STOREI" || com == "STOREF") {
					if(farg1[0]=='$') {
						assembly.push_back(CodeLine(line->scope, "move", getWhatever(farg1), getWhatever(farg2)));
				    }
						else{
							std::string newR = getNewReg();
							tempToReg[farg1] = newR;
							assembly.push_back(CodeLine(line->scope, "move", getWhatever(farg1), newR));
							assembly.push_back(CodeLine(line->scope, "move", newR,getWhatever(farg2)));
						}
				
				} else if(com == "WRITEI" || com == "WRITEF" || com == "WRITES") {
					assembly.push_back(CodeLine(line->scope, "sys", getLower(com), getWhatever(farg1)));
				
				} else if(com == "READI" || com == "READF") {
					assembly.push_back(CodeLine(line->scope, "sys", getLower(com), getWhatever(farg1)));
				
				} else {
					const std::string Ids[] = { "ADD", "SUB", "DIV", "MUL"};
					const std::string cmps[] = { "GE", "GT", "LT", "LE", "NE", "EQ"};
					bool presentinId = false;
					for(std::string i : Ids) {
						if(i == com.substr(0, 3)) {
							assembly.push_back(CodeLine(line->scope, "move", getWhatever(farg1), getWhatever(farg3)));						
							assembly.push_back(CodeLine(line->scope, getLower(com), getWhatever(farg2), getWhatever(farg3)));						
							presentinId = true;
							break;
						}
					}

					for(std::string i : cmps) {
						if(i == com) {
							if(line->arg2[0] != '$') {
								std::string ty = "";
								for (SymbolTable* table: STvector) {
									if (table->symbols.find(farg1) != table->symbols.end()) {
										Entry* ent = table->symbols[farg1];
										ty = ent->type;
									}
								}
								std::string temp = getRegForID(line->arg2);
								assembly.push_back(CodeLine(line->scope, "move", getWhatever(farg2), temp));
								if (ty == "FLOAT") {
									assembly.push_back(CodeLine(line->scope, "cmpr", getWhatever(farg1), temp));
								}
								else {
									assembly.push_back(CodeLine(line->scope, "cmpi", getWhatever(farg1), temp));
								}
							}
							else {
								std::string ty = "";
								for (SymbolTable* table: STvector) {
									if (table->symbols.find(farg1) != table->symbols.end()) {
										Entry* ent = table->symbols[farg1];
										ty = ent->type;
									}
								}
								std::string newR = getNewReg();
								assembly.push_back(CodeLine(line->scope, "move", getWhatever(farg2), newR));
								if (ty == "FLOAT") {
									assembly.push_back(CodeLine(line->scope, "cmpr", getWhatever(farg1), newR));
								}
								else {
									assembly.push_back(CodeLine(line->scope, "cmpi", getWhatever(farg1), newR));
								}
							}
						
							assembly.push_back(CodeLine(line->scope, "j" + getLower(com), getWhatever(farg3)));
						
							presentinId = true;
							break;
						}
					}

					if(com == "JUMP") {
						assembly.push_back(CodeLine(line->scope, "jmp", line->arg1));
						presentinId = true;
					} else if(com == "LABEL") {
						assembly.push_back(CodeLine(line->scope, "label", line->arg1));
						presentinId = true;
					}
				}
			}
        }

		void print() {
			for (auto line: assembly)
				line.print();
		}

};

#endif