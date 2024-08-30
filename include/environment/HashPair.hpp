#pragma once

#include"Object.hpp"

class HashPair:public Object{
public:
    shared_ptr<Object> _key;
    shared_ptr<Object> _value;
    HashPair(shared_ptr<Object> key,shared_ptr<Object> value):_key(key),_value(value){}
};