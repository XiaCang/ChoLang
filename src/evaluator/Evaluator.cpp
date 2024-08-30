#include"evaluator/Evaluator.h"



shared_ptr<Object> Evaluator::Eval(ASTNode *ast,Environment *env){
    switch (ast->Type())
    {
    case ASTNodeType::Program:
        return EvalProgram(*(dynamic_cast<Program*>(ast)),*env);


    case ASTNodeType::ExpressionStatement:
        return Eval((dynamic_cast<ExpressionStatement*>(ast))->_expression.get(),env); 


    case ASTNodeType::IntegerLiteral:
        return make_shared<Integer>((dynamic_cast<IntegerLiteral*>(ast))->_value);

    case ASTNodeType::Boolean:
        return make_shared<Boolean>((dynamic_cast<Bool*>(ast))->_value);


    case ASTNodeType::Identifier:
        return EvalIdentifier((dynamic_cast<Identifier*>(ast)),env);


    case ASTNodeType::PrefixExpression:{
        auto prefixExpr=(dynamic_cast<PrefixExpression*>(ast));
        auto right=Eval(prefixExpr->_right.get(),env);
        if(right->Type()==ObjectType::ERROR){
            return right;
        }
        return EvalPrefixExpression(prefixExpr->_operator,right.get());        
    }
    case ASTNodeType::InfixExpression:{
        auto infixExpr=(dynamic_cast<InfixExpression*>(ast));

        auto left=Eval(infixExpr->_left.get(),env);
        if(left->Type()==ObjectType::ERROR){
            return left;
        }

        auto r=Eval(infixExpr->_right.get(),env);
        if(r->Type()==ObjectType::ERROR){
            return r;
        }

        return EvalInfixExpression(infixExpr->_op,left.get(),r.get());        
    }

        

    case ASTNodeType::IfExpression:
        return EvalIfExpression((dynamic_cast<IfExpression*>(ast)),env);
        

    case ASTNodeType::BlockStatement:
        return EvalBlockStatement((dynamic_cast<BlockStatement*>(ast)),env);
        

    case ASTNodeType::FunctionLiteral:
        {        
            auto func=(dynamic_cast<FunctionLiteral*>(ast));
            auto params=func->_parameters;
            auto body=func->_body;
            return make_shared<Function>(params,body,env);
        }

    case ASTNodeType::CallExpression:{
        auto callExpr=(dynamic_cast<CallExpression*>(ast));

        auto args=EvalExpressionList(*(callExpr->_arguments),env);

        if(args[0]->Type()==ObjectType::ERROR&&args.size()==1)
            return args[0];

        
        auto callee=dynamic_cast<Identifier*>(callExpr->_callee.get());

        if(callee->_value=="print"){
            for (int i = 0; i < args.size(); i++)
            {
                cout<<args[i]->ToString()<<endl;
            }
            return make_shared<NULL_>();
        }else{
            auto function=Eval(callExpr->_callee.get(),env);

            if(function->Type()==ObjectType::ERROR)
                return function;
            
            return EvalFunction(function,args);
        }
        return make_shared<Error>("unknown error");

    }

    case ASTNodeType::ArrayLiteral:{
        auto array=(dynamic_cast<ArrayLiteral*>(ast));

        auto elems=EvalExpressionList(*(array->_elems),env);

        if(elems[0]->Type()==ObjectType::ERROR&&elems.size()==1){
            return elems[0];
        }
        return make_shared<Array>(elems);        
    }


    case ASTNodeType::IndexExpression:{
        auto indexExpr=(dynamic_cast<IndexExpression*>(ast));

        auto left=Eval(indexExpr->_left.get(),env);
 
        if(left->Type()==ObjectType::ERROR){
            return left;
        }

        auto index=Eval(indexExpr->_index.get(),env);
        if(index->Type()==ObjectType::ERROR){
            return index;
        }

        return EvalIndexExpression(left.get(),index.get());        
    }


    case ASTNodeType::HashLiteral:
        return EvalHashLiteral((dynamic_cast<HashLiteral*>(ast)),env);

    case ASTNodeType::ReturnStatement:{

        auto returnStmt=(dynamic_cast<ReturnStatement*>(ast));
        auto val = Eval(returnStmt->_ret.get(),env);

        if(val->Type()==ObjectType::ERROR){
            return val;
        }


        return make_shared<ReturnValue>(val);        
    }


        
    
    case ASTNodeType::VarStatement:{
        auto varStmt=(dynamic_cast<VarStatement*>(ast));
        auto val = Eval(varStmt->_value.get(),env);

        if(val->Type()==ObjectType::ERROR){
            return val;
        }

        env->Set(varStmt->_identifier->_value,val);     
        break;   
    }


        
    
    case ASTNodeType::StringLiteral:
        return make_shared<String>((dynamic_cast<StringLiteral*>(ast))->_value);
        

    default:
        return nullptr;
    }
    return nullptr;
}

shared_ptr<Object> Evaluator::EvalProgram(Program &program,Environment &env){
    auto result=make_shared<Object>();

    for (int i = 0; i < program._statements.size(); i++)
    {
        result=Eval(program._statements[i].get(),&env);

        if(auto returnStatement=dynamic_cast<ReturnValue*>(result.get())){
            return returnStatement->_value;
        }

        if(auto error=dynamic_cast<Error*>(result.get())){
            return shared_ptr<Object>(error);
        }

    }
    return result;

}

shared_ptr<Object> Evaluator::EvalBlockStatement(BlockStatement* block,Environment* env){
    auto result=make_shared<Object>();

    for (int i = 0; i < block->_statements.size(); i++)
    {
        result=Eval(block->_statements[i].get(),env);

        if(result){
            if(result->Type()==ObjectType::RETURNVALUE||result->Type()==ObjectType::ERROR){
                return result;
            }
        }

    }
    return result;
}


shared_ptr<Object> Evaluator::EvalPrefixExpression(string op,Object* obj){

   if(op=="-"){
        if(obj->Type()==ObjectType::INTEGER){
            auto integer=static_cast<Integer*>(obj);
            return make_shared<Integer>(-(integer->value));
        }else{
            return make_shared<Error>("unknown operator: - for "+obj->ObjName(obj->Type()));
        }
    }

    if(op=="!"){
        if(obj->Type()==ObjectType::BOOLEAN){
            auto boolean=static_cast<Boolean*>(obj);
            return make_shared<Boolean>(!(boolean->_val));
        }
        if(obj->Type()==ObjectType::NULL_){
            return make_shared<Boolean>(true);
        }

        return make_shared<Boolean>(false);
    }

    return make_shared<Error>("unknown operator: "+op+" for "+obj->ObjName(obj->Type()));
}


shared_ptr<Object> Evaluator::EvalInfixExpression(string op,Object* left,Object* right){
    if(left->Type()==ObjectType::INTEGER&&right->Type()==ObjectType::INTEGER){
        return EvalIntegerInfixExpression(op,left,right);
    }

    if(left->Type()==ObjectType::STRING&&right->Type()==ObjectType::STRING){
        return EvalStringInfixExpression(op,left,right);
    }

    if(op=="=="){
        auto l=static_cast<Boolean*> (left);
        auto r=static_cast<Boolean*> (right);

        return make_shared<Boolean>(l->_val==r->_val);
    }

    if(op=="!="){
        auto l=static_cast<Boolean*> (left);
        auto r=static_cast<Boolean*> (right);

        return make_shared<Boolean>(l->_val!=r->_val);
    }

    return make_shared<Error>("unknown operator: "+op+" for "+left->ObjName(left->Type())+" and "+right->ObjName(right->Type()));

}


shared_ptr<Object> Evaluator::EvalIntegerInfixExpression(string op,Object* left,Object* right){

    auto l=static_cast<Integer*> (left);
    auto r=static_cast<Integer*> (right);

    if(op=="+"){
        return make_shared<Integer>(l->value+r->value);
    }

    if(op=="-"){
        return make_shared<Integer>(l->value-r->value);
    }

    if(op=="*"){
        return make_shared<Integer>(l->value*r->value);
    }

    if(op=="/"){
        return make_shared<Integer>(l->value/r->value);
    }

    if(op=="<"){
        return make_shared<Boolean>(l->value<r->value);
    }

    if(op==">"){
        return make_shared<Boolean>(l->value>r->value);
    }

    if(op=="=="){
        return make_shared<Boolean>(l->value==r->value);
    }

    if(op=="!="){
        return make_shared<Boolean>(l->value!=r->value);
    }

    return make_shared<Error>("unknown operator: "+op+" for "+left->ObjName(left->Type())+" and "+right->ObjName(right->Type()));
}


shared_ptr<Object> Evaluator::EvalStringInfixExpression(string op,Object* left,Object* right){

    auto l=static_cast<String*> (left);
    auto r=static_cast<String*> (right);

    if(op=="+"){
        return make_shared<String>(l->_value+r->_value);
    }

    if(op=="=="){
        return make_shared<Boolean>(l->_value==r->_value);
    }

    if(op=="!="){
        return make_shared<Boolean>(l->_value!=r->_value);
    }

    return make_shared<Error>("unknown operator: "+op+" for "+left->ObjName(left->Type())+" and "+right->ObjName(right->Type()));
}


shared_ptr<Object> Evaluator::EvalIfExpression(IfExpression* expression,Environment* env){
    auto condition=Eval(expression->_condition.get(),env);
    if(condition->Type()==ObjectType::ERROR){
        return condition;
    }

    if(condition->Type()==ObjectType::BOOLEAN){
        auto b=static_cast<Boolean*>(condition.get());
        if(b->_val){
            return Eval(expression->_consequence.get(),env);
        }
        if(expression->_alternative.get()!=nullptr){
            return Eval(expression->_alternative.get(),env);
        }
        return make_shared<Object>(NULL_());
    }
    return make_shared<Error>("unknown error: "+condition->ObjName(condition->Type()));
}


shared_ptr<Object> Evaluator::EvalIdentifier(Identifier* identifier,Environment* env){
    auto value=env->Get(identifier->_value);

     
    if(value){
        return value;
    }
    return make_shared<Error>("identifier not found: "+identifier->_value);
}
shared_ptr<Object> Evaluator::EvalFunction(shared_ptr<Object> func,vector<shared_ptr<Object>> args){
    if(auto funcObj=dynamic_cast<Function*>(func.get())){
        auto exEnv=NewEnvironment(funcObj->_env);

        for(int i=0;i<funcObj->_parameters->size();i++){
            exEnv->Set(((*(funcObj->_parameters))[i])->_value,args[i]);
        }

        auto result=Eval(funcObj->_body.get(),exEnv.get());

        if(auto returnValue=dynamic_cast<ReturnValue*>(result.get())){
            return returnValue->_value;
        }


        return result;
    }

    return make_shared<Error>("not a function: "+func->ObjName(func->Type()));
}

vector<shared_ptr<Object>> Evaluator::EvalExpressionList(vector<shared_ptr<Expression>> exp,Environment* env){
    vector<shared_ptr<Object>> result;

    for (int i = 0; i < exp.size(); i++)
    {
        auto res=Eval(exp[i].get(),env);
        if(res->Type()==ObjectType::ERROR){
            return vector<shared_ptr<Object>>{res};
        }
        result.push_back(res);
    }

    return result;
}

shared_ptr<Object> Evaluator::EvalIndexExpression(Object* left,Object* index){

    if(left->Type()==ObjectType::ARRAY&&index->Type()==ObjectType::INTEGER){

        auto l=static_cast<Array*>(left);
        auto i=static_cast<Integer*>(index);

        if(i->value<0||i->value>=l->_elems.size()){
            return make_shared<Error>("array index out of range");
        }

        return l->_elems[i->value];
    }

    if(left->Type()==ObjectType::HASH){

        auto l=dynamic_cast<Hash*>(left);
        auto i=dynamic_cast<String*>(index);
        if(!i){
            return make_shared<Error>("unknown key: "+index->ObjName(index->Type())+" for "+left->ObjName(left->Type()));
        }
        return (*l->_pairs)[i->Hash()]->_value;
    }
}

shared_ptr<Object> Evaluator::EvalHashLiteral(HashLiteral* hash,Environment* env){
    auto pairs=make_shared<map<HashKey,shared_ptr<HashPair>>>();
    auto _pairs=*hash->_pairs;
    auto it=_pairs.begin();

    while(it!=_pairs.end()){
        auto key=Eval(it->first.get(),env);
        
        if(key->Type()==ObjectType::ERROR){
            return key;
        }

        IHashable *hashkey=dynamic_cast<IHashable*>(key.get());


        if(!hashkey){
            return make_shared<Error>("unusable as hash key: "+key->ObjName(key->Type()));
        }

        auto value=Eval(it->second.get(),env);
        if(value->Type()==ObjectType::ERROR){
            return value;
        }

        auto hashed=hashkey->Hash();
        (*pairs)[hashed]=make_shared<HashPair>(key,value);
        it++;

    }

    return make_shared<Hash>(pairs);
}
