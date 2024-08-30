#pragma once
#include"token/Token.h"


class Lexer{
    vector<char> str;

    int _curpos;
    int _nextpos;

    char _ch;

    int _line;
    int _col;


    void ReadChar();

    char PeekChar();

    void SkipWhitespace();

    string ReadChars(int n);

    string ReadString();

    string ReadIdentifier();

    string ReadNumber();

    bool IsLetter();

    bool IsDigit();

    

public:
    Lexer() = default;

    Lexer(const Lexer& other);

    Lexer(string code);

    Token NextToken();
};
