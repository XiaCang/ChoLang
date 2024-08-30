#pragma once
#include"Object.hpp"



class NULL_:public Object{

public:
    ObjectType Type() override{return ObjectType::NULL_;}
    string ToString() override{return "null";}
};
