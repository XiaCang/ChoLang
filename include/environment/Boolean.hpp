#pragma once
#include"Object.hpp"
#include"IHashable.hpp"

class Boolean : public Object,public IHashable{

public:
    bool _val;

    Boolean(bool val):_val(val){}

    ObjectType Type() override {return ObjectType::BOOLEAN;}

    std::string ToString() override {
        return _val ? "true" : "false";
    }

    bool operator==(const Boolean& rhs) const {
        return _val == rhs._val;
    }

    bool operator!=(const Boolean& rhs) const {
        return _val != rhs._val;
    }

    HashKey  Hash() override{
        return HashKey(ObjectType::BOOLEAN,_val?1:0);
    }

};