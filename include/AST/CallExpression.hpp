#pragma once

#include"./base/Expression.hpp"
#include"../token/Token.h"

class CallExpression : public Expression{

public:

    shared_ptr<Token> _token;
    shared_ptr<Expression> _callee;
    shared_ptr<vector<shared_ptr<Expression>>> _arguments;

    CallExpression(Token &token,shared_ptr<Expression> callee){
        _token=make_shared<Token>(token);
        _callee=callee;
    }

    string ToString() override
    {
        string s=_callee->ToString()+"(";
        for (int i = 0; i < _arguments->size(); i++)
        {
            s+=_arguments->at(i)->ToString();
            if(i!=_arguments->size()-1)
                s+=",";
        }
        s+=")";
        return s;
    }

    string Value() override
    {
        return _token->Value();
    }

    Json ToJson() override
    {
        Json j;
        j["type"] = "CallExpression";
        j["callee"] = _callee->ToJson();
        for (auto &&i : *_arguments)
        {
            j["arguments"].push_back(i->ToJson());
        }
        
        return j;
    }

    virtual ASTNodeType Type(){return ASTNodeType::CallExpression;}
};