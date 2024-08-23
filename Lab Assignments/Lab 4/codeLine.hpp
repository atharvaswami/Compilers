// Acknowledgement: https://github.com/aswanthpp/Compiler-Design

#ifndef CODELINE_HPP
#define CODELINE_HPP

#include <bits/stdc++.h>

using namespace std;

class CodeLine {

public:
    string command, arg1 ,arg2, arg3;
    string scope;

    CodeLine (string scope, string command, string arg1) {
        this->scope = scope;
        this->command = command;
        this->arg1 = arg1;
        this->arg2 = "";
        this->arg3 = "";
    }

    CodeLine (string scope, string command, string arg1, string arg2) {
        this->scope = scope;
        this->command = command;
        this->arg1 = arg1;
        this->arg2 = arg2;
        this->arg3 = "";
    }

    CodeLine (string scope, string command, string arg1, string arg2, string arg3) {
        this->scope = scope;
        this->command = command;
        this->arg1 = arg1;
        this->arg2 = arg2;
        this->arg3 = arg3;
    }

    void print() {
        string print_val =  command + " " + arg1;

        if (arg2 != "")
            print_val += " " + arg2;

        if (arg3 != "")
            print_val += " " + arg3;
            
        cout << print_val << endl;
    }

};

#endif
