#pragma once

#include<iostream>
#include<memory>
#include"json/json.hpp"
using namespace std;
using Json=nlohmann::json;


enum class ASTNodeType{
    Base,
    Expression,
    Statement,
    Program,
    ArrayLiteral,
    BlockStatement,
    FunctionLiteral,
    Identifier,
    IntegerLiteral,
    PrefixExpression,
    InfixExpression,
    Boolean,
    ExpressionStatement,
    IfExpression,
    ReturnStatement,
    VarStatement,
    CallExpression,
    HashLiteral,
    IndexExpression,
    StringLiteral
};



class ASTNode{

public:
    virtual string Value()=0;

    virtual string ToString()=0;

    virtual Json ToJson()=0;

    virtual ASTNodeType Type()=0;
};