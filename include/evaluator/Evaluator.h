#pragma once

#include"../parser/Parser.h"
#include"environment/Environment.hpp"
#include"environment/ReturnValue.hpp"
#include"environment/Error.hpp"
#include"environment/Integer.hpp"
#include"environment/Boolean.hpp"
#include"environment/String.hpp"
#include"environment/Function.hpp"
#include"environment/Array.hpp"
#include"environment/NULL.hpp"
#include"environment/Hash.hpp"
class Evaluator{
public:
    Evaluator()=default;
    shared_ptr<Object> Eval(ASTNode *ast,Environment *env);
    shared_ptr<Object> EvalProgram(Program &program,Environment &env);
    shared_ptr<Object> EvalBlockStatement(BlockStatement* block,Environment* env);
    shared_ptr<Object> EvalPrefixExpression(string op,Object* obj);
    shared_ptr<Object> EvalInfixExpression(string op,Object* left,Object* right);
    shared_ptr<Object> EvalIfExpression(IfExpression* expression,Environment* env);
    shared_ptr<Object> EvalIdentifier(Identifier* identifier,Environment* env);
    shared_ptr<Object> EvalFunction(shared_ptr<Object> func,vector<shared_ptr<Object>> args);
    vector<shared_ptr<Object>> EvalExpressionList(vector<shared_ptr<Expression>> exp,Environment* env);
    shared_ptr<Object> EvalIndexExpression(Object* left,Object* index);
    shared_ptr<Object> EvalHashLiteral(HashLiteral* hash,Environment* env);
    shared_ptr<Object> EvalIntegerInfixExpression(string op,Object* left,Object* right);
    shared_ptr<Object> EvalStringInfixExpression(string op,Object* left,Object* right);
};