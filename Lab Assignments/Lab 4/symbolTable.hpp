// Acknowledgement: https://github.com/aswanthpp/Compiler-Design

#ifndef SYMBOL_TABLE_HPP
#define SYMBOL_TABLE_HPP

#include <bits/stdc++.h>
#include "entry.hpp"

using namespace std;

class SymbolTable {

public:
    string scope;
    unordered_map<string, Entry *> symbols;
    vector<Entry *> ordered_symbols;
    int total_parameters = 0;
    int total_non_parameters = 0;

    SymbolTable (string scope) {
        this->scope = scope;
    }

    int linkSize() {
        return total_non_parameters;
    }

    Entry* findEntry (string name) {
        return symbols[name];
    }

    void addEntry (string name, string type) {
        total_non_parameters++;
        Entry* variable = new Entry(name, type);
        variable->stackname = "$-" + to_string(total_non_parameters);
        ordered_symbols.push_back(variable);
        symbols[name] = variable;
        cout << "var " << name << endl;
    }

    void addEntry (string name, string type, string value) {
        total_non_parameters++;
        Entry* variable = new Entry(name, type, value);
        variable->stackname = value;
        ordered_symbols.push_back(variable);
        symbols[name] = variable;

        if(value ==""){
            cout << "var " << name << endl;
        }

        else{
            cout << "str " << name << " " << value << endl;
        }
    }

    void addEntry (string name, string type, bool isParameter) {
        total_parameters++;
        Entry* variable = new Entry(name, type, isParameter);
        variable->stackname = "$" + to_string(total_parameters+1);
        ordered_symbols.push_back(variable);
        symbols[name] = variable;
        cout<<"var "<<name<<endl;
    }

    bool ifExists (string name) {
        if (symbols.find(name) == symbols.end())
            return false;
        else
            return true;
    }

    void printTable() {
        cout << "Symbol table " << scope << endl;

        for (auto it = ordered_symbols.begin(); it != ordered_symbols.end(); ++it) {
            cout << "name " << (*it)->name << " type " << (*it)->type;    
            if ((*it)->value != "")
                cout << " value " << (*it)->value;
            cout << endl;
        }
    }
};

#endif
