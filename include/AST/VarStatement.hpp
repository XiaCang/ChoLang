#pragma once

#include"./base/Statement.hpp"
#include"./base/Expression.hpp"
#include"../token/Token.h"
#include"Identifier.hpp"
class VarStatement:public Statement{

public:
    shared_ptr<Token> _token;
    shared_ptr<Identifier> _identifier;
    shared_ptr<Expression> _value;

    VarStatement(VarStatement &varStatement){
        _token=make_shared<Token>(*(varStatement._token));
        _identifier=varStatement._identifier;
        _value=varStatement._value;
    }

    VarStatement(Token &token){
        _token=make_shared<Token>(token);
    }

    string ToString() override
    {
        return Value()+" "+_identifier->ToString()+"="+_value->ToString()+";";
    }
    string Value() override
    {
        return _token->Value();
    }

    Json ToJson() override
    {
        Json j;
        j["type"] = "VarStatement";
        j["identifier"] = _identifier->ToJson();
        j["value"] = _value->ToJson();
        return j;
    }

    virtual ASTNodeType Type(){return ASTNodeType::VarStatement;}
};