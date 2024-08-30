#pragma once
#include"Object.hpp"
#include"IHashable.hpp"


class Integer:public Object,public IHashable{
public:
    int64_t value;

    Integer(int64_t value):value(value){}

    ObjectType Type() override{return ObjectType::INTEGER;}

    string ToString() override{return to_string(value);}

    HashKey  Hash() override{
        return HashKey(ObjectType::INTEGER,uint64_t(value));
    }
};