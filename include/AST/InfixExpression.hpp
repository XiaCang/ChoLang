#pragma once
#include"./base/Expression.hpp"
#include"../token/Token.h"

class InfixExpression : public Expression {
  public:
    shared_ptr<Token> _token; // // The operator token, e.g. +
    shared_ptr<Expression> _left;
    string _op;
    shared_ptr<Expression> _right;

    InfixExpression(Token &token, string op, shared_ptr<Expression> l)
        : _token(make_shared<Token>(token)), _op(op), _left(l){};

    string Value() override 
    { 
        return _token->Value(); 
    };
    string  ToString() override 
    {
        string ret = _left->ToString() + " " + _op + " " +
                     _right->ToString() ;

        return ret;
    };

    Json ToJson() override{
        Json j;
        j["type"] = "InfixExpression";
        j["left"] = _left->ToJson();
        j["op"] = _op;
        j["right"] = _right->ToJson();
        return j;
    }

    virtual ASTNodeType Type(){return ASTNodeType::InfixExpression;}
};
