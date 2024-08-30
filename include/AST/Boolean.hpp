#pragma once

#include"./base/Expression.hpp"
#include"../token/Token.h"

class Bool : public Expression
{

public:
    shared_ptr<Token> _token;
    bool _value;

    Bool(Token token, bool value)
    {
        _token = make_shared<Token>(token);
        _value = value;
    }

    string ToString() override
    {
        return _token->Value();
    }

    string Value() override
    {
        return _token->Value();
    }
    Json ToJson() override
    {
        Json j;
        j["type"] = "Boolean";
        j["value"] = _value;
        return j;
    }

    virtual ASTNodeType Type(){return ASTNodeType::Boolean;}

};