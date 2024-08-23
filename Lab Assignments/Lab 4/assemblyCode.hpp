// Acknowledgement: https://github.com/aswanthpp/Compiler-Design

#ifndef ASSEMBLYCODE_HPP
#define ASSEMBLYCODE_HPP

#include <bits/stdc++.h>
#include "codeObject.hpp"
#include "symbolTable.hpp"

using namespace std;

class AssemblyCode {
	
    vector<CodeLine *> assembly;
    int register_no = -1;
    unordered_map<string, string> temp_to_reg;
	
public:
    
    string getRegister(string temporary) {   
        if (tempExists(temporary)) {
            return temp_to_reg.find(temporary)->second;
		}
        else {
            string reg = "r" + to_string(++register_no);
            this->temp_to_reg[temporary] = reg;
            return reg;
        }
    }

    string getNewRegister() {
        return "r" + to_string(++register_no);
    }

    bool isTemporary(string temp) {
        return (temp[0] == '$');
    }

    bool tempExists(string temp) {
        return (temp_to_reg.find(temp) != temp_to_reg.end());
    }

	string toLower(string s) {
		for (char& i : s) {
			i |= ' ';
		}

		if (s.back() == 'f') {
			s.back() = 'r';
		}

		return s;
	}

    void generateCode(CodeObject *code, SymbolTableStack *tableStack) {
        vector<CodeLine *> threeAC = code->threeAC;
	    for (auto code_line : threeAC) {
	        string command = code_line->command;
	        string new_arg1 = code_line->arg1;
	        string new_arg2 = code_line->arg2;

		    if (command == "READI") {
		        assembly.push_back(new CodeLine(code_line->scope, "sys", "readi", new_arg1));
		    }

            else if (command == "READF") {
		        assembly.push_back(new CodeLine(code_line->scope, "sys", "readr", new_arg1));
		    }
		            
			else if (command == "WRITEI") {
		        assembly.push_back(new CodeLine(code_line->scope, "sys", "writei", new_arg1));
		    }
		            
			else if (command == "WRITEF") {
	            assembly.push_back(new CodeLine(code_line->scope, "sys", "writer", new_arg1));
		    }
		    
			else if (command == "WRITES") {
		        assembly.push_back(new CodeLine(code_line->scope, "sys", "writes", new_arg1));
		    }
		            
			else if (command == "STOREI" || command == "STOREF") {
		        new_arg1 = isTemporary(code_line->arg1) ? getRegister(code_line->arg1) : new_arg1;
		        new_arg2 = isTemporary(code_line->arg2) ? getRegister(code_line->arg2) : new_arg2;
		        assembly.push_back(new CodeLine(code_line->scope, "move", new_arg1, new_arg2));
		    }
		            
			else {
				bool found = false;
				string ops[] = {"GE", "GT", "LT", "LE", "NE", "EQ"};
				string tem_arg1 = isTemporary(code_line->arg1) ? getRegister(code_line->arg1) : code_line->arg1;
				string tem_arg2 = isTemporary(code_line->arg2) ? getRegister(code_line->arg2) : code_line->arg2;
				string tem_arg3 = isTemporary(code_line->arg3) ? getRegister(code_line->arg3) : code_line->arg3;

				for (auto i:ops) {
					if (i == command) {
						assembly.push_back(new CodeLine(code_line->scope, "cmpi", tem_arg1, tem_arg2));
						assembly.push_back(new CodeLine(code_line->scope, "j"+toLower(command), tem_arg3));
						found = true;
						break;
					}
				}

				if (command == "JUMP") {
					assembly.push_back(new CodeLine(code_line->scope, "jmp", code_line->arg1));
					found = true;
				}
						
				else if (command == "LABEL") {
					assembly.push_back(new CodeLine(code_line->scope, "label", code_line->arg1));
					found = true;
				}

				if (found) {
					continue;
				}

				string command;
		        new_arg1 = isTemporary(code_line->arg1) ? getRegister(code_line->arg1) : new_arg1;
		        string arg2 = getNewRegister();
		        assembly.push_back(new CodeLine(code_line->scope, "move", new_arg1, arg2));

		        if (code_line->command == "ADDI")
		            command = "addi";
		        else if (code_line->command == "SUBI")
		            command = "subi";
		        else if (code_line->command == "MULTI")
		            command = "muli";
		        else if (code_line->command == "DIVI")
		            command = "divi";
		        else if (code_line->command == "ADDF")
		            command = "addr";
		        else if (code_line->command == "SUBF")
		            command = "subr";
		        else if (code_line->command == "MULTF")
		            command = "mulr";
		        else if (code_line->command == "DIVF")
		            command = "divr";

		        new_arg1 = isTemporary(code_line->arg2) ? getRegister(code_line->arg2) : new_arg2;
		        assembly.push_back(new CodeLine(code_line->scope, command, new_arg1, arg2));

		        if (isTemporary(code_line->arg3)) {
					temp_to_reg[code_line->arg3] = arg2;
				}
		    }
	    }
    }

    void printMap() {
        for (auto it: temp_to_reg)
            cout << it.first << " " << it.second << endl;
        cout << "PRINTED" << endl;
    }

    void print() {
        for (auto c: assembly)
            c->print();
    }

};

#endif