#pragma once

#include"Node.hpp"


class Expression:public ASTNode{

public:
    virtual string ToString(){return "";};
    virtual string Value(){return "";};
    virtual Json ToJson(){return Json();};
    virtual ASTNodeType Type(){return ASTNodeType::Expression;};
};