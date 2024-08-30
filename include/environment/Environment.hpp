#pragma once
#include"Object.hpp"
#include<map>

class Environment:public enable_shared_from_this<Environment>{
public:
    map<string,shared_ptr<Object>> _vars;
    shared_ptr<Environment> _outer;

    shared_ptr<Environment> GetThisPtr(){
        return shared_from_this();
    }

    shared_ptr<Object> Get(string name){
        if(_vars.count(name)){
            return _vars[name];
        }
        if(_outer){
            return _outer->Get(name);
        }
        return shared_ptr<Object>();
    }

    shared_ptr<Object> Set(string name,shared_ptr<Object> value){
        _vars[name]=value;
        return value;
    }

    Environment(){}

    Environment(shared_ptr<Environment> outer):_outer(outer){}
};

inline shared_ptr<Environment> NewEnvironment(){
    return make_shared<Environment>();
}

inline shared_ptr<Environment> NewEnvironment(shared_ptr<Environment> outer){
    return make_shared<Environment>(outer);
}

