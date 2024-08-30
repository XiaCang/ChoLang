#pragma once
#include"./base/Expression.hpp"
#include"../token/Token.h"


class ArrayLiteral:public Expression{

public:
    shared_ptr<vector<shared_ptr<Expression>>> _elems;
    shared_ptr<Token> _token;

    ArrayLiteral(Token &token) 
            :_token(&token) {}

    string Value() override
        {return _token->Value();}

    string ToString() override
    {
        string s="[";   
        for (int i = 0; i < _elems->size(); i++)
        {
            s+=_elems->at(i)->ToString();
            if(i!=_elems->size()-1)
                s+=",";
        }
        s+="]";
        return s;

    }

    Json ToJson() override
    {
        Json j;
        j["type"] = "ArrayLiteral";
        for (auto &&i : *_elems)
        {
            j["elems"].push_back(i->ToJson());
        }
        
        return j;
    }

    virtual ASTNodeType Type(){return ASTNodeType::ArrayLiteral;}
};