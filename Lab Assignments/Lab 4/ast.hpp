// Acknowledgement: https://github.com/aswanthpp/Compiler-Design

#ifndef AST_HPP
#define AST_HPP

#include <bits/stdc++.h>
#include "entry.hpp"
#include "codeObject.hpp"

using namespace std;

class ASTNode {

public:
    static string id_type;
    string type;
    ASTNode *left, *right;

    virtual string generateCode(CodeObject *code) {
        return "N/A";
    };
};

class ASTNode_Expr : public ASTNode {

public:
    string type = "EXPR";
    char optype;

    ASTNode_Expr(char optype) {
        this->optype = optype;
    }

    string generateCode(CodeObject *code) {
        string left = this->left->generateCode(code);
        string right = this->right->generateCode(code);
        string temp = code->getTemp();
        string command = "";

        if (ASTNode::id_type == "INT") {
            if (optype == '+')
                command += "ADDI";
            else if (optype == '-')
                command += "SUBI";
            else if (optype == '*')
                command += "MULTI";
            else if (optype == '/')
                command += "DIVI";
        }

        else if (ASTNode::id_type == "FLOAT") {
            if (optype == '+')
                command += "ADDF";
            else if (optype == '-')
                command += "SUBF";
            else if (optype == '*')
                command += "MULTF";
            else if (optype == '/')
                command += "DIVF";
        }

        code->threeAC.push_back(new CodeLine(code->symbolTableStack->table_stack.top()->scope, command, left, right, temp));
        return temp;
    }
};

class ASTNode_INT : public ASTNode {

public:
    string type = "INT";
    int value;

    ASTNode_INT(int value) {
        this->value = value;
    }

    string generateCode(CodeObject *code) {
        string temp = code->getTemp();
        code->threeAC.push_back(new CodeLine(code->symbolTableStack->table_stack.top()->scope, "STOREI", to_string(this->value), temp));
        return temp;
    }
};

class ASTNode_FLOAT : public ASTNode {

public:
    string type = "FLOAT";
    float value;

    ASTNode_FLOAT(float value) {
        this->value = value;
    }

    string generateCode(CodeObject *code) {
        string temp = code->getTemp();
        char str[10];
        sprintf(str, "%f", value);
        code->threeAC.push_back(new CodeLine(code->symbolTableStack->table_stack.top()->scope, "STOREF", str, temp));
        return temp;
    }
};

class ASTNode_ID : public ASTNode {

public:
    string type = "ID";
    Entry *variable;

    ASTNode_ID(Entry *variable) {
        this->variable = variable;
    }

    string generateCode(CodeObject *code) {
        return this->variable->name;
    }
};

class ASTNode_Assign : public ASTNode {

public:
    string type = "ASSIGN";

    ASTNode_Assign(Entry* var) {
        this->left = new ASTNode_ID(var);
        ASTNode::id_type = var->type;
    }

    string generateCode(CodeObject *code) {
        string command = "";

        if (ASTNode::id_type == "INT")
            command += "STOREI";
        else if (ASTNode::id_type == "FLOAT")
            command += "STOREF";

        code->threeAC.push_back(new CodeLine(code->symbolTableStack->table_stack.top()->scope,
                                             command, 
                                             this->right->generateCode(code),
                                             this->left->generateCode(code)));
        return "";
    }
};

class ASTNode_Cond : public ASTNode {

public:
    string comp_op;

    ASTNode_Cond(string comp_op) {
        this->comp_op = comp_op;
    }

    string generateCode(CodeObject *code) {
        string op, label;
        string arg1 = this->left->generateCode(code);
        string arg2 = this->right->generateCode(code);

        code->lb += 1;
        code->lbList.emplace_back(code->lb);
        label = "LABEL" + to_string(code->lb);

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

        code->threeAC.push_back(new CodeLine(code->symbolTableStack->table_stack.top()->scope, op, arg1, arg2, label));
        return "";
    }
};

#endif
