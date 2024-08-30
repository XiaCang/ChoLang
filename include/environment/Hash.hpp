#pragma once
#include"Object.hpp"
#include"HashKey.hpp"
#include"HashPair.hpp"
#include<map>
class Hash:public Object{
public:
    shared_ptr<map<HashKey,shared_ptr<HashPair>>> _pairs;

    Hash(shared_ptr<map<HashKey,shared_ptr<HashPair>>> p):_pairs(p){
        
    }

    ObjectType Type() override{return ObjectType::HASH; }

    string ToString() override{
        string s="{";
        auto &pairs=*_pairs;

        auto it=pairs.begin();
        while (it!=pairs.end())
        {
            auto hash_pair=it->second;
            auto key=hash_pair->_key;
            auto value=hash_pair->_value;
            s+=key->ToString()+":"+value->ToString();
            it++;
        }

        s+="}";

        return s;
        
    }

};