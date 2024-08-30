#pragma once

#include"./base/Statement.hpp"
#include"./base/Expression.hpp"
#include"../token/Token.h"

class ReturnStatement:public Statement{
public:
    shared_ptr<Token> _token;
    shared_ptr<Expression> _ret;
    ReturnStatement(Token &token){
        _token=make_shared<Token>(token);
    }

    string ToString() override
    {
        return Value()+_ret->ToString()+";";
    }
    
    string Value() override
    {
        return _token->Value();
    }

    Json ToJson() override
    {
        Json j;
        j["type"] = "ReturnStatement";
        j["value"] = _ret->ToJson();
        return j;
    }


    virtual ASTNodeType Type(){return ASTNodeType::ReturnStatement;}
};