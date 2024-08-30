#pragma once

#include"./base/Statement.hpp"
#include"./base/Expression.hpp"
#include"../token/Token.h"

class IfExpression : public Expression
{
public:
    shared_ptr<Token> _token;
    shared_ptr<Expression> _condition;
    shared_ptr<Statement> _consequence;
    shared_ptr<Statement> _alternative;

    IfExpression(Token &token):_token(make_shared<Token>(token)){}

    string ToString() override{
        string s="if("+_condition->ToString()+") "+_consequence->ToString();
        if(_alternative.get()!=nullptr)
            s+="\nelse\n"+_alternative->ToString();
        return s;
    }

    string Value() override
    {
        return _token->Value();
    }

    Json ToJson() override
    {
        Json j;
        j["type"] = "IfExpression";
        j["condition"] = _condition->ToJson();
        j["consequence"] = _consequence->ToJson();
        if(_alternative.get()!=nullptr)
            j["alternative"] = _alternative->ToJson();
        return j;
    }

    virtual ASTNodeType Type(){return ASTNodeType::IfExpression;}
};