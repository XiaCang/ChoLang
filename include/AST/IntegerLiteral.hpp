#pragma once

#include"./base/Expression.hpp"
#include"../token/Token.h"

class IntegerLiteral : public Expression
{
public:
    shared_ptr<Token> _token;
    int64_t _value;

    IntegerLiteral(Token &token) : _token(make_shared<Token>(token)) {}
    IntegerLiteral(Token &token, int64_t value) : _token(make_shared<Token>(token)), _value(value) {}

    string ToString() override
    {
        return to_string(_value);
    }

    string Value() override
    {
        return _token->Value();
    }

    Json ToJson() override
    {
        Json j;
        j["type"] = "IntegerLiteral";
        j["value"] = _value;
        return j;
    }


    virtual ASTNodeType Type(){return ASTNodeType::IntegerLiteral;}
};