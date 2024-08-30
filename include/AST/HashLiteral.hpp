#pragma once

#include"./base/Expression.hpp"
#include"../token/Token.h"

class HashLiteral : public Expression
{
public:
    shared_ptr<Token> _token;
    shared_ptr<map<shared_ptr<Expression>, shared_ptr<Expression>>> _pairs;
    HashLiteral(Token &token):_token(new Token(token)){}

    string ToString() override
    {
        string s="{";
        int size=_pairs->size();
        int cnt=0;
        for (auto it = _pairs->begin(); it != _pairs->end(); it++)
        {
            s+=it->first->ToString()+":"+it->second->ToString();
            if(++cnt!=size)
                s+=",";
        }
        s+="}";
        return s;
    }
    string Value() override
    {
        return _token->Value();
    }

    Json ToJson() override{
        Json j;
        j["type"] = "HashLiteral";
        for (auto &&i : *_pairs)
        {
            Json j2;
            j2["key"] = i.first->ToJson();
            j2["value"] = i.second->ToJson();
            j["pairs"].push_back(j2);
        }
        
        return j;
    }

    virtual ASTNodeType Type(){return ASTNodeType::HashLiteral;}
};