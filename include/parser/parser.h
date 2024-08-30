#pragma once

#include"lexer/Lexer.h"
#include"AST/base/Expression.hpp"
#include"AST/Program.hpp"
#include"AST/VarStatement.hpp"
#include"AST/ReturnStatement.hpp"
#include"AST/BlockStatement.hpp"
#include"AST/ExpressionStatement.hpp"
#include"AST/InfixExpression.hpp"
#include"AST/PrefixExpression.hpp"
#include"AST/IntegerLiteral.hpp"
#include"AST/Boolean.hpp"
#include"AST/IfExpression.hpp"
#include"AST/FunctionLiteral.hpp"
#include"AST/CallExpression.hpp"
#include"AST/StringLiteral.hpp"
#include"AST/ArrayLiteral.hpp"
#include"AST/IndexExpression.hpp"
#include"AST/HashLiteral.hpp"
#include<functional>
enum class Precedence{
    LOWEST,
    EQUALS,      // ==
    LESSGREATER, // > or <
    SUM,         // +
    PRODUCT,     // *
    PREFIX,      // -X or !X
    CALL,        // myFunction(X)
    INDEX        // array[index]
};

const map<TokenType,Precedence> tokenPrecedence={
    {TokenType::EQ,Precedence::EQUALS},
    {TokenType::NOT_EQ,Precedence::EQUALS},
    {TokenType::LT,Precedence::LESSGREATER},
    {TokenType::GT, Precedence::LESSGREATER},
    {TokenType::PLUS, Precedence::SUM},
    {TokenType::MINUS, Precedence::SUM},
    {TokenType::SLASH, Precedence::PRODUCT},
    {TokenType::ASTERISK, Precedence::PRODUCT},
    {TokenType::LPAREN, Precedence::CALL},
    {TokenType::LBRACKET, Precedence::INDEX}
};

class Parser{
    Lexer _lexer;
    vector<string> errors;

    Token _curToken;
    Token _peekToken;

    using prefixParseFn = function<shared_ptr<Expression>()>;
    using infixParseFn = function<shared_ptr<Expression>(shared_ptr<Expression>)>;
    map<TokenType, prefixParseFn> _prefixParseFns;
    map<TokenType, infixParseFn> _infixParseFns;


    void Init();

    void AddPrefixFunc(TokenType type, prefixParseFn fn);
    void AddInfixFunc(TokenType type, infixParseFn fn);

    void NextToken();
    
    bool CurTokenIs(TokenType type);
    bool PeekTokenIs(TokenType type);
    bool ExpectPeek(TokenType type);

    void AddError(string msg);

    Precedence CurPrecedence();
    Precedence PeekPrecedence();

    shared_ptr<Statement> ParseStatement();
    shared_ptr<VarStatement> ParseVarStatement();
    shared_ptr<ReturnStatement> ParseReturnStatement();
    shared_ptr<ExpressionStatement> ParseExpressionStatement();
    shared_ptr<Expression> ParseExpression(Precedence precedence);
    shared_ptr<Expression> ParseIdentifier();
    shared_ptr<Expression> ParseIntegerLiteral();
    shared_ptr<Expression> ParsePrefixExpression();
    shared_ptr<Expression> ParseInfixExpression(shared_ptr<Expression> left);
    shared_ptr<Expression> ParseBoolean();
    shared_ptr<Expression> ParseGroupedExpression();
    shared_ptr<Expression> ParseIfExpression();
    shared_ptr<Expression> ParseFunctionLiteral();
    shared_ptr<Expression> ParseCallExpression(shared_ptr<Expression> function);
    shared_ptr<Expression> ParseArrayLiteral();
    shared_ptr<Expression> ParseIndexExpression(shared_ptr<Expression> left);
    shared_ptr<Expression> ParseHashLiteral();
    shared_ptr<Expression> ParseStringLiteral();
    shared_ptr<BlockStatement> ParseBlockStatement();
    shared_ptr<vector<shared_ptr<Identifier>>> ParseFunctionParameters();
    shared_ptr<vector<shared_ptr<Expression>>> ParseExpressionList(TokenType end);
public:
    Parser(Lexer lexer);

    shared_ptr<Program> ParseProgram();

    vector<string> Errors();
};