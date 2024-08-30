#pragma once

#include"./base/Expression.hpp"
#include"../token/Token.h"

class Identifier:public Expression{

public:
    shared_ptr<Token> _token;
    string _value;

    Identifier() = default;

    Identifier(Identifier &identifier){
        _token = make_shared<Token>(*(identifier._token.get()));
        _value = identifier._value;
    }

    Identifier(Token &token, string value){
        _token = make_shared<Token>(token);
        _value = value;
    }

    string ToString() override
    {
        return _value;
    }

    string Value() override
    {
        return _token->Value();
    }

    Json ToJson() override
    {
        Json j;
        j["type"] = "Identifier";
        j["value"] = _value;
        return j;
    }

    virtual ASTNodeType Type(){return ASTNodeType::Identifier;}
};