#pragma once

#include<iostream>
#include<vector>
#include<string>
#include<memory>
using namespace std;


enum class ObjectType{
    NULL_,
    BOOLEAN,
    INTEGER,
    STRING,
    ARRAY,
    ERROR,
    HASH,
    FUNCTION,
    BUILTIN,
    RETURNVALUE
};

const string ObjectTypeName[] = {"NULL_","BOOLEAN","NUMBER","STRING","ARRAY","ERROR","HASH","FUNCTION","BUILTIN","RETURNVALUE"};


class Object{

public:
    virtual ObjectType Type(){return ObjectType::NULL_;};
    virtual string ToString(){return "";}
    static string ObjName(ObjectType type){return ObjectTypeName[static_cast<int>(type)];}
};