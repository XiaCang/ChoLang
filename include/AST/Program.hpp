#pragma once

#include"./base/Statement.hpp"
#include"./base/Expression.hpp"
#include"../token/Token.h"

class Program : public ASTNode
{
public:
    vector<shared_ptr<Statement>> _statements;

    string Value() override{
        if(_statements.size()>0)
            return _statements[0]->Value();
        return "";
    }

    string ToString() override
    {
        string s="";   
        for (int i = 0; i < _statements.size(); i++)
            s+=_statements[i]->ToString();
        return s;
    }

    Json ToJson() override
    {
        Json j;
        for (int i = 0; i < _statements.size(); i++)
            j["Program"].push_back(_statements[i]->ToJson());
        return j;
    }

    virtual ASTNodeType Type(){return ASTNodeType::Program;}
};