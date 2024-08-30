#pragma once

#include"./base/Statement.hpp"
#include"./base/Expression.hpp"
#include"BlockStatement.hpp"
#include"Identifier.hpp"
#include"../token/Token.h"

class FunctionLiteral : public Expression
{
public:
    shared_ptr<Token> _token;
    shared_ptr<vector<shared_ptr<Identifier>>> _parameters;
    shared_ptr<BlockStatement> _body;


    FunctionLiteral(Token &token){
        _token=make_shared<Token>(token);
    }

    string ToString() override
    {
        string ret="";
        ret+=Value();
        ret+="(";
        for (int i = 0; i < _parameters->size(); i++)
        {
           ret+=_parameters->at(i)->ToString();
           if(i< _parameters->size()-1)
                ret+=", ";
        }
        ret+=") {";
        ret+= _body->ToString();
        ret+="}";
        return ret;
    }

    string Value() override
    {
        return _token->Value();
    }

    Json ToJson() override{
        Json j;
        j["type"] = "FunctionLiteral";
        for (auto &&i : *_parameters)
        {
            j["parameters"].push_back(i->ToJson());
        }
        
        j["body"] = _body->ToJson();
        return j;
    }

    virtual ASTNodeType Type(){return ASTNodeType::FunctionLiteral;}
};