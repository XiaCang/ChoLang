#pragma once

#include"Node.hpp"

class Statement:public ASTNode{
public:
    virtual string ToString()=0;
    virtual string Value()=0;
    virtual Json ToJson()=0;
    virtual ASTNodeType Type()=0;
};