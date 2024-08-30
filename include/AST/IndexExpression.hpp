#pragma once

#include"./base/Statement.hpp"
#include"./base/Expression.hpp"
#include"../token/Token.h"

class IndexExpression : public Expression
{
public:
    shared_ptr<Token> _token;
    shared_ptr<Expression> _left;
    shared_ptr<Expression> _index;

    IndexExpression(Token& token){
        _token=make_shared<Token>(token);
    }

    IndexExpression(Token &token, shared_ptr<Expression> left){
        _token=make_shared<Token>(token);
        _left=left;
    }

    string ToString() override{
        return _left->ToString()+"["+_index->ToString()+"]";
    }

    string Value() override
    {
        return _token->Value();
    }

    Json ToJson() override
    {
        Json j;
        j["type"] = "IndexExpression";
        j["left"] = _left->ToJson();
        j["index"] = _index->ToJson();
        return j;
    }
    virtual ASTNodeType Type(){return ASTNodeType::IndexExpression;}

};