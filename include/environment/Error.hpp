#pragma once
#include"Object.hpp"
class Error:public Object{
public:
    string msg;

    Error()=default;

    Error(string msg):msg(msg){}

    ObjectType Type() override{return ObjectType::ERROR;}

    string ToString() override{return "Error: "+msg;}

};