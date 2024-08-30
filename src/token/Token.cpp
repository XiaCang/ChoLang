#include"token/Token.h"

map<string,TokenType> Token::keywords={
    {"fn",TokenType::FUNCTION},
    {"var",TokenType::VAR},
    {"true",TokenType::TRUE_},
    {"false",TokenType::FALSE_},
    {"if",TokenType::IF},
    {"else",TokenType::ELSE},
    {"return",TokenType::RETURN}
};

Token::Token(){

}

Token::Token(const Token& other){
    _type=other._type;
    _value=other._value;
    _line=other._line;
    _col=other._col;
}

Token::Token(TokenType type, string value,int line,int col){
    _type=type;
    _value=value;
    _line=line;
    _col=col;
}

TokenType Token::Type() const{
    return _type;
}

string Token::Value() const{
    return _value;
}

string Token::ToString() const{
    return TokenNames[static_cast<int>(_type)];
}