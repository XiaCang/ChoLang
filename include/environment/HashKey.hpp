#pragma once

#include"Object.hpp"


class HashKey:public Object{

public:
    ObjectType _type;
    uint64_t _val;

    HashKey()=default;

    HashKey(const HashKey &key):_type(key._type),_val(key._val){}

    HashKey(ObjectType type,uint64_t val):_type(type),_val(val){}

    bool operator==(const HashKey &key)const{
        return _type==key._type&&_val==key._val;
    }

    bool operator!=(const HashKey &key)const{
        return _type!=key._type||_val!=key._val;
    }

    bool operator<(const HashKey &key)const{
        return (key._val>_val);
    }

};