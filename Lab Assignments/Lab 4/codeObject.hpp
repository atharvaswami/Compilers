// Acknowledgement: https://github.com/aswanthpp/Compiler-Design

#ifndef CodeObject_HPP
#define CodeObject_HPP

#include <bits/stdc++.h>
#include "codeLine.hpp"
#include "symbolTableStack.hpp"

using namespace std;

class CodeObject {
    int temp_value = 0;

public:
    vector<CodeLine*> threeAC;
    SymbolTableStack* symbolTableStack;
    int lb = 0;
    deque<int> lbList;
    
    CodeObject (SymbolTableStack* symbolTableStack) {
        this->symbolTableStack = symbolTableStack;
    }

    string getTemp() {
        return "$T" + to_string(++temp_value);
    }

    void addRead (string var_name, string type) {
        if (type == "INT")
            threeAC.push_back(new CodeLine(symbolTableStack->table_stack.top()->scope, "READI", var_name));

        else if (type == "FLOAT")
            threeAC.push_back(new CodeLine(symbolTableStack->table_stack.top()->scope, "READF", var_name));
    }

    void addWrite(string var_name, string type) {
        if (type == "INT")
            threeAC.push_back(new CodeLine(symbolTableStack->table_stack.top()->scope, "WRITEI", var_name));

        else if (type == "FLOAT")
            threeAC.push_back(new CodeLine(symbolTableStack->table_stack.top()->scope, "WRITEF", var_name));

        else if (type == "STRING")
            threeAC.push_back(new CodeLine(symbolTableStack->table_stack.top()->scope, "WRITES", var_name));
    }

    void print() {
        for (auto s: threeAC)
            s->print();
    }

};

#endif
