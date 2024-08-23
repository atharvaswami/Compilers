// Acknowledgement: https://github.com/aswanthpp/Compiler-Design

#ifndef ENTRY_HPP
#define ENTRY_HPP

#include <bits/stdc++.h>

using namespace std;

class Entry {

public:
    string name, type, value;
    string stackname;
    bool isParameter = false;

    Entry (string name, string type) {
        this->name = name;
        this->type = type;
        this->value = "";
    }
    
    Entry (string name, string type, string value) {
        this->name = name;
        this->type = type;
        this->value = value;
    }

    Entry (string name, string type, bool isParameter) {
        this->name = name;
        this->type = type;
        this->isParameter = isParameter;
    }

};

#endif
