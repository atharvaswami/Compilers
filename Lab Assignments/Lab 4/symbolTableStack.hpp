// Acknowledgement: https://github.com/aswanthpp/Compiler-Design

#ifndef SYMBOL_TABLE_STACK_HPP
#define SYMBOL_TABLE_STACK_HPP

#include <bits/stdc++.h>
#include "symbolTable.hpp"

using namespace std;

class SymbolTableStack {
    string error_variable = "";

public:
    vector<SymbolTable *> tables;
    stack<SymbolTable *> table_stack;
    int block_number = 1;

    void addNewTable() {
        SymbolTable *temp = new SymbolTable("BLOCK " + to_string(block_number));
        table_stack.push(temp);
        tables.push_back(temp);
        block_number += 1;
    }

    void addNewTable (string name) {
        SymbolTable *temp = new SymbolTable(name);
        table_stack.push(temp);
        tables.push_back(temp);
    }

    void removeTable() {
        table_stack.pop();
    }

    void insertSymbol (string name, string type) {
        SymbolTable *table = table_stack.top();

        if (table->ifExists(name) && error_variable == "")
            error_variable = name;
        else
            table->addEntry(name, type);
    }

    void insertSymbol (string name, string type, string value) {
        SymbolTable *table = table_stack.top();

        if (table->ifExists(name) && error_variable == "")
            error_variable = name;
        else
            table->addEntry(name, type, value);
    }

    void insertSymbol (string name, string type, bool isParameter) {
        SymbolTable *table = table_stack.top();
        
        if (table->ifExists(name) && error_variable == "")
            error_variable = name;
        else
            table->addEntry(name, type, isParameter);
    }

    Entry* findEntry (string name) {
        stack<SymbolTable *> temp_stack = table_stack;

        while (temp_stack.size()) {
            if (temp_stack.top()->ifExists(name))
                return temp_stack.top()->findEntry(name);
            temp_stack.pop();
        }

        return new Entry("error", "error");
    }

    string findType(string name) {
        return findEntry(name)->type;
    }

    void printStack() {
        if (error_variable != "")
            cout << "DECLARATION ERROR " + error_variable << endl;
            
        else {
            for (int i = 0; i != tables.size(); i++) {
                tables[i]->printTable();
                if (i != tables.size() - 1)
                    cout << endl;
            }
        }
    }
};

#endif
