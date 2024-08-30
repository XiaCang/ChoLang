#include"parser/parser.h"

//Parser::
Parser::Parser(Lexer lexer):_lexer(lexer){

    Init();
}

void Parser::AddPrefixFunc(TokenType type, prefixParseFn fn){
    _prefixParseFns[type] = fn;
}
void Parser::AddInfixFunc(TokenType type, infixParseFn fn){
    _infixParseFns[type] = fn;
}


void Parser::NextToken(){
    _curToken = _peekToken;
    _peekToken = _lexer.NextToken();
}

void Parser::Init(){
    AddPrefixFunc(TokenType::IDENT, bind(&ParseIdentifier,this));
    AddPrefixFunc(TokenType::INT, bind(&ParseIntegerLiteral,this));
    AddPrefixFunc(TokenType::BANG, bind(&ParsePrefixExpression,this));
    AddPrefixFunc(TokenType::MINUS, bind(&ParsePrefixExpression,this));
    AddPrefixFunc(TokenType::TRUE_, bind(&ParseBoolean,this));
    AddPrefixFunc(TokenType::FALSE_, bind(&ParseBoolean,this));
    AddPrefixFunc(TokenType::LPAREN, bind(&ParseGroupedExpression,this));
    AddPrefixFunc(TokenType::IF, bind(&ParseIfExpression,this));
    AddPrefixFunc(TokenType::FUNCTION, bind(&ParseFunctionLiteral,this));
    AddPrefixFunc(TokenType::STRING, bind(&ParseStringLiteral,this));
    AddPrefixFunc(TokenType::LBRACKET, bind(&ParseArrayLiteral,this));
    AddPrefixFunc(TokenType::LBRACE, bind(&ParseHashLiteral,this));


    AddInfixFunc(TokenType::EQ, bind(&ParseInfixExpression,this,placeholders::_1));
    AddInfixFunc(TokenType::NOT_EQ, bind(&ParseInfixExpression,this,placeholders::_1));
    AddInfixFunc(TokenType::LT, bind(&ParseInfixExpression,this,placeholders::_1));
    AddInfixFunc(TokenType::GT, bind(&ParseInfixExpression,this,placeholders::_1));
    AddInfixFunc(TokenType::PLUS, bind(&ParseInfixExpression,this,placeholders::_1));
    AddInfixFunc(TokenType::MINUS, bind(&ParseInfixExpression,this,placeholders::_1));
    AddInfixFunc(TokenType::SLASH, bind(&ParseInfixExpression,this,placeholders::_1));
    AddInfixFunc(TokenType::ASTERISK, bind(&ParseInfixExpression,this,placeholders::_1));
    AddInfixFunc(TokenType::LPAREN, bind(&ParseCallExpression,this,placeholders::_1));
    AddInfixFunc(TokenType::LBRACKET, bind(&ParseIndexExpression,this,placeholders::_1));
    NextToken();
    NextToken();
}
vector<string> Parser::Errors(){
    return errors;
}

void Parser::AddError(string msg){
    errors.push_back(msg);
}

bool Parser::CurTokenIs(TokenType type){
    return _curToken.Type() == type;
}
bool Parser::PeekTokenIs(TokenType type){
    return _peekToken.Type() == type;
}
bool Parser::ExpectPeek(TokenType type){
    if(PeekTokenIs(type)){
        NextToken();
        return true;
    }
    
    return false;
}

Precedence Parser::CurPrecedence(){
    auto it=tokenPrecedence.find(_curToken.Type());
    if(it != tokenPrecedence.end()){
        return it->second;
    }
    return Precedence::LOWEST;
}
Precedence Parser::PeekPrecedence(){
    auto it=tokenPrecedence.find(_peekToken.Type());
    if(it != tokenPrecedence.end()){
        return it->second;
    }
    return Precedence::LOWEST;
}

shared_ptr<Program> Parser::ParseProgram(){
    //程序根节点
    shared_ptr<Program> program = make_shared<Program>();


    while(!CurTokenIs(TokenType::EOF_)){
        auto statement = ParseStatement();
        if(statement)
            program->_statements.push_back(statement);
        //?
         NextToken(); 
    }

    return program;

}

shared_ptr<Statement> Parser::ParseStatement(){
    switch (_curToken.Type())
    {
    case TokenType::VAR:
        return ParseVarStatement();
    case TokenType::RETURN:
        return ParseReturnStatement();
    default:
        return ParseExpressionStatement();
    }
    return nullptr;
}


shared_ptr<VarStatement> Parser::ParseVarStatement(){
    auto stmt=make_shared<VarStatement>(_curToken);

    if(!ExpectPeek(TokenType::IDENT)){
        AddError("expected next token to be IDENT in line "+to_string(_curToken._line)+" col "+to_string(_curToken._col));
        return nullptr;
    }

    stmt->_identifier = make_shared<Identifier>(_curToken,_curToken.Value());
    if(PeekTokenIs(TokenType::SEMICOLON)){
        return stmt;
    }

    if(!ExpectPeek(TokenType::ASSIGN)){
        AddError("expected next token to be ASSIGN in line "+to_string(_curToken._line)+" col "+to_string(_curToken._col));
        return nullptr;
    }
    NextToken();


    stmt->_value = ParseExpression(Precedence::LOWEST);

    if(stmt->_value == nullptr){
        AddError("expected expression in line "+to_string(_curToken._line)+" col "+to_string(_curToken._col));
        return nullptr;
    }

    if(ExpectPeek(TokenType::SEMICOLON)){
        return stmt;
    }
    AddError("expected next token to be ';' in line "+to_string(_curToken._line)+" col "+to_string(_curToken._col));
    return nullptr;
}

shared_ptr<ReturnStatement> Parser::ParseReturnStatement(){

    auto stmt = make_shared<ReturnStatement>(_curToken);

    NextToken();

    auto expression = ParseExpression(Precedence::LOWEST);

    if(expression == nullptr){
        AddError("expected expression in line "+to_string(_curToken._line)+" col "+to_string(_curToken._col));
        return nullptr;
    }

    stmt->_ret = expression;

    if(ExpectPeek(TokenType::SEMICOLON)){
        return stmt;
    }

    AddError("expected next token to be ';' in line "+to_string(_curToken._line)+" col "+to_string(_curToken._col));
    return nullptr;
}

shared_ptr<ExpressionStatement> Parser::ParseExpressionStatement(){
    auto es = make_shared<ExpressionStatement>(_curToken);

    auto exp=ParseExpression(Precedence::LOWEST);
    if(exp == nullptr){
        AddError("expected expression in line "+to_string(_curToken._line)+" col "+to_string(_curToken._col));
        return nullptr;
    }

    es->_expression = exp;

    if(PeekTokenIs(TokenType::SEMICOLON)){
        NextToken();
        
    }

    return es;
}


shared_ptr<Expression> Parser::ParseExpression(Precedence precedence){
    auto it=_prefixParseFns.find(_curToken.Type());
    if(it == _prefixParseFns.end()){
        AddError("no prefix parse function for "+TokenNames[static_cast<int>(_curToken.Type())]);
        return nullptr;
    }

    auto left=it->second();



    while(!CurTokenIs(TokenType::SEMICOLON) && precedence < PeekPrecedence()){
        auto it=_infixParseFns.find(_peekToken.Type());
        if(it == _infixParseFns.end()){
            AddError("no infix parse function for "+TokenNames[static_cast<int>(_curToken.Type())]);
            return left;
        }
        NextToken();
        auto left2 = it->second(left);
        left = left2;
    }
    return left;
}

shared_ptr<Expression> Parser::ParseInfixExpression(shared_ptr<Expression> left){
    auto exp = make_shared<InfixExpression>(_curToken, _curToken.Value(), left);

    Precedence precedence = CurPrecedence();
    NextToken();

    exp->_right = ParseExpression(precedence);

    return exp;
}

shared_ptr<Expression> Parser::ParsePrefixExpression(){
    auto exp = make_shared<PrefixExpression>(_curToken, _curToken.Value());
    NextToken();

    exp->_right = ParseExpression(Precedence::PREFIX);

    return exp;
}

shared_ptr<Expression> Parser::ParseIdentifier(){
    return make_shared<Identifier>(_curToken,_curToken.Value());
}

shared_ptr<Expression> Parser::ParseIntegerLiteral(){
    return make_shared<IntegerLiteral>(_curToken,_strtoi64(_curToken.Value().c_str(), nullptr, 10));
}

shared_ptr<Expression> Parser::ParseBoolean(){
    return make_shared<Bool>(_curToken,CurTokenIs(TokenType::TRUE_));
}

shared_ptr<Expression> Parser::ParseGroupedExpression(){
    NextToken();

    auto exp = ParseExpression(Precedence::LOWEST);

    if(!ExpectPeek(TokenType::RPAREN)){
        AddError("expected next token to be ')' in line "+to_string(_curToken._line)+" col "+to_string(_curToken._col));
        return nullptr;
    }

    return exp;
}

//if
shared_ptr<Expression> Parser::ParseIfExpression(){

    auto exp = make_shared<IfExpression>(_curToken);

    if(!ExpectPeek(TokenType::LPAREN)){
        AddError("expected next token to be '(' in line "+to_string(_curToken._line)+" col "+to_string(_curToken._col));
        return nullptr;
    }

    NextToken();

    exp->_condition = ParseExpression(Precedence::LOWEST);

    if(!ExpectPeek(TokenType::RPAREN)){
        AddError("expected next token to be ')' in line "+to_string(_curToken._line)+" col "+to_string(_curToken._col));
        return nullptr;
    }

    if(!ExpectPeek(TokenType::LBRACE)){
        AddError("expected next token to be '{' in line "+to_string(_curToken._line)+" col "+to_string(_curToken._col));
        return nullptr;
    }

    exp->_consequence = ParseBlockStatement();

    if(PeekTokenIs(TokenType::ELSE)){
        NextToken();
        if(!ExpectPeek(TokenType::LBRACE)){
            exp->_alternative = ParseStatement();
        }else{
            exp->_alternative = ParseBlockStatement();
        }
    }

    return exp;

}

shared_ptr<BlockStatement> Parser::ParseBlockStatement(){
    auto block = make_shared<BlockStatement>(_curToken);

    NextToken();

    while(!CurTokenIs(TokenType::RBRACE) && !CurTokenIs(TokenType::EOF_)){
        auto stmt = ParseStatement();
        if(stmt){
            block->_statements.push_back(stmt);
        }
        NextToken();
    }

    return block;
}

shared_ptr<Expression> Parser::ParseFunctionLiteral(){

    auto lit = make_shared<FunctionLiteral>(_curToken);

    

    if(!ExpectPeek(TokenType::LPAREN)){
        AddError("expected next token to be '(' in line "+to_string(_curToken._line)+" col "+to_string(_curToken._col));
        return nullptr;
    }

    lit->_parameters = ParseFunctionParameters();

    if(!ExpectPeek(TokenType::LBRACE)){
        AddError("expected next token to be '{' in line "+to_string(_curToken._line)+" col "+to_string(_curToken._col));
        return nullptr;
    }

    lit->_body = ParseBlockStatement();

    return lit;
}

shared_ptr<vector<shared_ptr<Identifier>>> Parser::ParseFunctionParameters(){

    auto identifiers = make_shared<vector<shared_ptr<Identifier>>>();

    if(PeekTokenIs(TokenType::RPAREN)){
        NextToken();
        return identifiers;
    }

    NextToken();

    auto ident = make_shared<Identifier>(_curToken,_curToken.Value());

    identifiers->push_back(ident);

    while(PeekTokenIs(TokenType::COMMA)){
        NextToken();
        NextToken();
        auto ident = make_shared<Identifier>(_curToken,_curToken.Value());
        identifiers->push_back(ident);
    }

    if(!ExpectPeek(TokenType::RPAREN)){
        AddError("expected next token to be ')' in line "+to_string(_curToken._line)+" col "+to_string(_curToken._col));
        return nullptr;
    }

    return identifiers;

}

shared_ptr<Expression> Parser::ParseCallExpression(shared_ptr<Expression> function){

    auto exp = make_shared<CallExpression>(_curToken, function);

    exp->_arguments = ParseExpressionList(TokenType::RPAREN);

    return exp;
}
shared_ptr<vector<shared_ptr<Expression>>> Parser::ParseExpressionList(TokenType end){

    auto list = make_shared<vector<shared_ptr<Expression>>>();

    if(PeekTokenIs(end)){
        NextToken();
        return list;
    }

    NextToken();
    auto exp=ParseExpression(Precedence::LOWEST);
    list->push_back(exp);

    while(PeekTokenIs(TokenType::COMMA)){
        NextToken();
        NextToken();
        exp=ParseExpression(Precedence::LOWEST);
        list->push_back(exp);
    }

    if(!ExpectPeek(end)){
        AddError("expected next token to be "+TokenNames[static_cast<int>(end)]+" in line "+to_string(_curToken._line)+" col "+to_string(_curToken._col));
        return nullptr;
    }

    return list;

}


shared_ptr<Expression> Parser::ParseStringLiteral(){
    return make_shared<StringLiteral>(_curToken,_curToken.Value());
}

shared_ptr<Expression> Parser::ParseArrayLiteral(){
    auto array = make_shared<ArrayLiteral>(_curToken);
    array->_elems=ParseExpressionList(TokenType::RBRACKET);
    return array;
}

shared_ptr<Expression> Parser::ParseIndexExpression(shared_ptr<Expression> left){
    auto exp = make_shared<IndexExpression>(_curToken, left);

    NextToken();
    exp->_index = ParseExpression(Precedence::LOWEST);

    if(!ExpectPeek(TokenType::RBRACKET)){
        AddError("expected next token to be ']' in line "+to_string(_curToken._line)+" col "+to_string(_curToken._col));
        return nullptr;
    }
    return exp;
}


shared_ptr<Expression> Parser::ParseHashLiteral(){
    auto hash = make_shared<HashLiteral>(_curToken);

    hash->_pairs = make_shared<map<shared_ptr<Expression>, shared_ptr<Expression>>>();

    while (!PeekTokenIs(TokenType::RBRACE))
    {
        NextToken();

        auto key = ParseExpression(Precedence::LOWEST);

        if(!ExpectPeek(TokenType::COLON)){
            AddError("expected next token to be ':' in line "+to_string(_curToken._line)+" col "+to_string(_curToken._col));
            return nullptr;
        }

        NextToken();

        auto value = ParseExpression(Precedence::LOWEST);

        hash->_pairs->insert({key, value});

        bool _R=PeekTokenIs(TokenType::RBRACE);
        bool _C=ExpectPeek(TokenType::COMMA);
        //bool _E=_R||_C;
        if(!_R && !_C){
            AddError("expected next token to be ',' or '}' in line "+to_string(_curToken._line)+" col "+to_string(_curToken._col));
            return nullptr;
        }
    }

    if(!ExpectPeek(TokenType::RBRACE)){
        AddError("expected next token to be '}' in line "+to_string(_curToken._line)+" col "+to_string(_curToken._col));
        return nullptr;
    }

    return hash;
}