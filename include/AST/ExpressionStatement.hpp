#pragma once

#include"./base/Statement.hpp"
#include"./base/Expression.hpp"
#include"../token/Token.h"

class ExpressionStatement : public Statement
{
public:
    shared_ptr<Token> _token;
    shared_ptr<Expression> _expression;

    ExpressionStatement(Token &token){
        _token=make_shared<Token>(token);
    }

    string ToString() override{
        return _expression->ToString();
    }

    string Value() override
    {
        return _token->Value();
    }

    Json ToJson() override{
        Json j;
        j["type"] = "ExpressionStatement";
        j["value"] = _expression->ToJson();
        return j;
    }

    virtual ASTNodeType Type(){return ASTNodeType::ExpressionStatement;}

};