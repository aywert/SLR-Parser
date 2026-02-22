#ifndef LEXER_HPP
#define LEXER_HPP

#include <FlexLexer.h>
#include <queue>
#include "token.hpp"

class MyLexer : public yyFlexLexer {
  std::vector<Token> tokenBuf;
public:
  MyLexer(std::istream* in) : yyFlexLexer(in) { GetTokenBuffer(); }

  // Переопределяем метод yylex для получения токенов
  virtual int yylex() override {
    return yyFlexLexer::yylex();
  }

  Token getNextToken() {
    int token_t = yylex();  // Получаем код токена от Flex
    
    TokenType tokenType = static_cast<TokenType>(token_t); // Преобразуем код в тип токена
    // Вручную создаем структуру Token
    std::string name = yytext; 
    if (tokenType == IDENTIFIER) name = "id";

    return Token(
      tokenType,              // код токена
      yytext,                 // текст из Flex
      name.c_str(),
      yylineno                // номер строки
    );
  }

  void GetTokenBuffer() {
    Token t = getNextToken(); 
    tokenBuf.push_back(t);
    while (t.check()) {
      t = getNextToken();
      tokenBuf.push_back(t);
    }
  }

  std::vector<Token> getReadyBuf() {return tokenBuf;}

  void showTokens() {
    for(const auto& token: tokenBuf) {
      token.Dump();
    }
  }
};

#endif