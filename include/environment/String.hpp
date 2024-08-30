#pragma once
#include"Object.hpp"
#include"IHashable.hpp"


class String:public Object,public IHashable{
public:
    string _value;
    String(string value):_value(value){}
    ObjectType Type() override{return ObjectType::STRING;}
    string ToString() override{return _value;}
    HashKey Hash() override{
        auto h=hash<string>{}(_value);
        return HashKey(ObjectType::STRING,h);
    }
};