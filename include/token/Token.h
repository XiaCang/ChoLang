#pragma once
#include<iostream>
#include<string>
#include<map>
#include<vector>
#include<memory>
#include<functional>
using namespace std;

enum class TokenType {
    ILLEGAL, // 非法
    EOF_,    // 文件结束


    IDENT,  // 标识符.
    INT,    // 整数
    STRING, // 字符串

    // Operators
    ASSIGN,   // "="
    PLUS,     // "+"
    MINUS,    // "-"
    BANG,     // "!"
    ASTERISK, // "*"
    SLASH,    // "/"

    LT, // "<"
    GT, // ">"

    EQ,     // "=="
    NOT_EQ, // "!="

    // Delimiters
    COMMA,     // ","
    SEMICOLON, // ";"
    COLON,     // ":"

    LPAREN,   // "("
    RPAREN,   // ")"
    LBRACE,   // "{"
    RBRACE,   // "}"
    LBRACKET, // "["
    RBRACKET, // "]"
    // Keywords
    FUNCTION, // 函数
    VAR,      // 定义变量
    TRUE_,    // "TRUE"
    FALSE_,   // "FALSE"
    IF,       // "IF"
    ELSE,     // "ELSE"
    RETURN,   // "RETURN"
};

const string TokenNames[] = {
"ILLEGAL", "EOF",

    // Identifiers + literals
    "IDENT",  // add, foobar, x, y, ...,
    "INT",    // 1343456,
    "STRING", // "foobar"

    // Operators
    "ASSIGN",   // "="
    "PLUS",     // "+"
    "MINUS",    // "-"
    "BANG",     // "!"
    "ASTERISK", // "*"
    "SLASH",    // "/"

    "LT", // "<"
    "GT", // ">"

    "EQ",     // "=="
    "NOT_EQ", // "!="

    // Delimiters
    "COMMA",     // ","
    "SEMICOLON", // ";"
    "COLON",      //= ":"
    "LPAREN",    // "("
    "RPAREN",    // ")"
    "LBRACE",    // "{"
    "RBRACE",    // "}"
    "LBRACKET",  // "["
    "RBRACKET",  // "]"

    // Keywords
    "FUNCTION", // "FUNCTION"
    "VAR",      // "VAR"
    "TRUE_",    // "TRUE"
    "FALSE_",   // "FALSE"
    "IF",       // "IF"
    "ELSE",     // "ELSE"
    "RETURN",   // "RETURN"
};

class Token {
    TokenType _type;
    string _value;

public:   
    int _line;
    int _col;
    
    static map<string,TokenType> keywords;
    
    Token();

    Token(const Token& other);

    Token(TokenType type, string value,int line,int col);

    TokenType Type() const;

    string Value() const;

    string ToString() const;
    
};