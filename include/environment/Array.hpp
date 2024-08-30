#pragma once
#include"Object.hpp"


class Array : public Object{

public:

    vector<shared_ptr<Object>> _elems;

    Array(){}

    Array(vector<shared_ptr<Object>> elems){_elems=elems;}

    ObjectType Type() override{return ObjectType::ARRAY;}

    string ToString() override{
        string s="[";
        for (int i = 0; i < _elems.size(); i++)
        {
            s+=_elems[i]->ToString();
            if(i!=_elems.size()-1)
                s+=",";
        }
        s+="]";
        return s;
    }

};