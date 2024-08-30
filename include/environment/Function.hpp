#pragma once
#include"Object.hpp"
#include"Environment.hpp"
#include"parser/parser.h"
class Function:public Object{
public:
    shared_ptr<vector<shared_ptr<Identifier>>> _parameters;
    shared_ptr<BlockStatement> _body;
    shared_ptr<Environment> _env;


    Function(shared_ptr<vector<shared_ptr<Identifier>>> parameters,
            shared_ptr<BlockStatement> body,
            Environment* env)
    :_parameters(parameters),_body(body),_env(env){}




    ObjectType Type() override{return ObjectType::FUNCTION;}

    string ToString() override{
        string s="function(){";
        s+=_body->ToString();
        s+="}";
        return s;
    }

};