#pragma once

#include"./base/Statement.hpp"
#include"../token/Token.h"
#include"VarStatement.hpp"

class BlockStatement:public Statement
{
public:
    vector<shared_ptr<Statement>> _statements;
    shared_ptr<Token>  _token;

    BlockStatement(Token token){
        _token=make_shared<Token>(token);
    }

    BlockStatement(BlockStatement &other){
        _token=make_shared<Token>(*(other._token.get()));
        for (int i = 0; i < other._statements.size(); i++)
        {
            auto statement=other._statements[i].get();

            if(auto varStatement=dynamic_cast<VarStatement*>(statement))
                _statements.push_back(make_shared<VarStatement>(*varStatement));
        }
        
    }

    void AddStatement(shared_ptr<Statement> statement){
        _statements.push_back(statement);
    }

    string ToString() override
    {
        string s="";   
        for (int i = 0; i < _statements.size(); i++)
        {
            s+=_statements[i]->ToString();
            if(i!=_statements.size()-1)
                s+="\n";
        }
        return s;
    }

    string Value() override
    {
        return _token->Value();
    }

    Json ToJson() override
    {
        Json j;
        j["type"] = "BlockStatement";
        for (auto &&i : _statements)    
        {
            j["statements"].push_back(i->ToJson());
        }
        return j;

    }

    virtual ASTNodeType Type(){
        return ASTNodeType::BlockStatement;
    }
};