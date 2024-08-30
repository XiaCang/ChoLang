#pragma once

#include"./base/Expression.hpp"
#include"../token/Token.h"
class StringLiteral:public Expression
{
    public:
        shared_ptr<Token> _token;
        string _value;

        StringLiteral(Token token,string value){
            _token = make_shared<Token>(token);
            _value = value;
        }

        string ToString() override
            {return _token->Value();}

        string Value() override
            {return _token->Value();}

        Json ToJson() override
        {
            Json j;
            j["type"] = "StringLiteral";
            j["value"] = _value;
            return j;
        }

        virtual ASTNodeType Type(){return ASTNodeType::StringLiteral;}
};