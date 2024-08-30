#pragma once

#include"./base/Statement.hpp"
#include"./base/Expression.hpp"
#include"../token/Token.h"

class PrefixExpression : public Expression
{
public:
    shared_ptr<Token> _token;
    string _operator;
    shared_ptr<Expression> _right;

    PrefixExpression(Token &token, string op)
        : _token(make_shared<Token>(token)), _operator(op){}

    string ToString() override
    {
        return _operator + _right->ToString();
    }

    string Value() override
    {
        return _token->Value();
    }

    Json ToJson() override
    {
        Json j;
        j["type"] = "PrefixExpression";
        j["op"] = _operator;
        j["right"] = _right->ToJson();
        return j;
    }

    virtual ASTNodeType Type(){
        return ASTNodeType::PrefixExpression;}
};