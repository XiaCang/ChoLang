#include"lexer/lexer.h"

Lexer::Lexer(const Lexer& other)
    : str(other.str),         // 拷贝字符串内容
      _curpos(other._curpos), // 拷贝当前字符位置
      _nextpos(other._nextpos), // 拷贝下一个字符位置
      _ch(other._ch),         // 拷贝当前字符
      _line(other._line),     // 拷贝当前行
      _col(other._col)     // 拷贝当前列

{
    
}
Lexer::Lexer(string code)
    : str(code.begin(), code.end()), // 将字符串转为字符向量
      _curpos(0),                    // 初始化当前字符位置为0
      _nextpos(0),                   // 初始化下一个字符位置为0
      _ch(0),                        // 当前字符初始化为0（假设没有字符）
      _line(1),                      // 初始化行号为1（假设从第1行开始）
      _col(1)                      // 初始化列号为1（假设从第1列开始）

{
    if (!str.empty()) {
        _ch = str[0]; // 如果字符串不为空，初始化当前字符为第一个字符
        _nextpos = 1; // 初始化下一个字符位置为1
    }
}

void Lexer::SkipWhitespace(){
    while (_ch == ' ' || _ch == '\t' || _ch == '\n' || _ch == '\r')
        ReadChar();
}


void Lexer::ReadChar(){
    if(_nextpos < str.size()){
        _ch = str[_nextpos];
    }else{
        _ch = 0;
    }
    _curpos = _nextpos;
    _nextpos++;

    _col++;
    if(_ch == '\n'){
        _line++;
        _col = 1;
    }

}

string Lexer::ReadChars(int n){
    string res;
    for(int i = 0; i < n; i++){
        res += _ch;
        ReadChar();
    }
    return res;
}

string Lexer::ReadIdentifier(){
    string res;
    while(IsLetter() || IsDigit()){
        res += _ch;
        ReadChar();
    }
    return res;
}

string Lexer::ReadString(){
    string res;
    ReadChar();
    while(_ch != '"'){
        res += _ch;
        ReadChar();
    }
    ReadChar();
    return res;
}

string Lexer::ReadNumber(){
    string num;
    while(IsDigit()){
        num+=_ch;
        ReadChar();
    }
    return num;
}

bool Lexer::IsLetter(){
    return (_ch>='a' && _ch<='z') || (_ch>='A' && _ch<='Z') || _ch=='_';
}

bool Lexer::IsDigit(){
    return _ch>='0' && _ch<='9';
}

char Lexer::PeekChar(){
    if(_nextpos>=str.size())
        return 0;
    return str[_nextpos];
}

Token Lexer::NextToken(){
    SkipWhitespace();
    Token t;
    switch(_ch){
        case '+':
            t = Token(TokenType::PLUS, ReadChars(1), _line, _col);

            break;

        case '-':
            t = Token(TokenType::MINUS, ReadChars(1),_line, _col);

            break;

        case '*':
            t = Token(TokenType::ASTERISK, ReadChars(1),_line, _col);
            
            break;

        case '/':
            t = Token(TokenType::SLASH, ReadChars(1),_line, _col);
            
            break;
        
        case '=':
            if(PeekChar()=='='){
                t = Token(TokenType::EQ, ReadChars(2),_line, _col);
                
                
            }
            else{
                t = Token(TokenType::ASSIGN, ReadChars(1),_line, _col);
                
            }
            break;
        
        case '!':
            if(PeekChar()=='='){
                t = Token(TokenType::NOT_EQ, ReadChars(2),_line, _col);
                
            }
            else{
                t = Token(TokenType::BANG, ReadChars(1),_line, _col);
                
            }
            break;
        
        case '<':
            t = Token(TokenType::LT,ReadChars(1),_line, _col);
            
            break;

        case '>':
            t = Token(TokenType::GT, ReadChars(1),_line, _col);
            

            break;
        case ':':
            t = Token(TokenType::COLON, ReadChars(1),_line, _col);
            
            break;


        case ';':
            t = Token(TokenType::SEMICOLON, ReadChars(1),_line, _col);
            
            break;

        case ',':
            t = Token(TokenType::COMMA, ReadChars(1),_line, _col);
            
            break;

        case '(':
            t = Token(TokenType::LPAREN, ReadChars(1),_line, _col);
            
            break;

        case ')':
            t = Token(TokenType::RPAREN, ReadChars(1),_line, _col);
            
            break;
        
        case '{':
            t = Token(TokenType::LBRACE, ReadChars(1),_line, _col);
            
            break;
        
        case '}':
            t = Token(TokenType::RBRACE,ReadChars(1),_line, _col);
            
            break;

        case '[':
            t = Token(TokenType::LBRACKET, ReadChars(1),_line, _col);
            
            break;
        
        case ']':
            t = Token(TokenType::RBRACKET, ReadChars(1),_line, _col);
            
            break;

        case '"':
            t = Token(TokenType::STRING, ReadString(),_line, _col);
            break;

        case 0:
            t = Token(TokenType::EOF_, "",_line, _col);
            
            break;

        default:
            if(IsLetter()){
                string str = ReadIdentifier();
                auto it = Token::keywords.find(str);
                if(it!=Token::keywords.end()){
                    t = Token(it->second, str,_line, _col);
                }else{
                    t = Token(TokenType::IDENT, str,_line, _col);
                }
            }
            else if(IsDigit()){
                t = Token(TokenType::INT, ReadNumber(),_line, _col);
            }
            else{
                t = Token(TokenType::ILLEGAL, "",_line, _col);
            }
            break;
    }

    
    return t;


}
    
