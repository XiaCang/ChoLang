#pragma once
#include"Object.hpp"
#include"HashKey.hpp"
#include"HashPair.hpp"
class IHashable{
public:
    virtual HashKey Hash(){return HashKey();};
};