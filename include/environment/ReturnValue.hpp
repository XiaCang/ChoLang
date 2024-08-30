#pragma once
#include"Object.hpp"


class ReturnValue :public Object{

public:

    shared_ptr<Object> _value;
    ReturnValue(shared_ptr<Object> value):_value(value){}

    ObjectType Type() override{return ObjectType::RETURNVALUE;}

    string ToString() override{return _value->ToString();}
};
