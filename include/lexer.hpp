#ifndef LEXER_HPP
#define LEXER_HPP

#include <FlexLexer.h>
#include <queue>
#include "token.hpp"

class MyLexer : public yyFlexLexer {
public:
  MyLexer(std::istream* in) : yyFlexLexer(in) {}

  // Переопределяем метод yylex для получения токенов
  virtual int yylex() override {
    return yyFlexLexer::yylex();
  }

  Token getNextToken() {
    std::cout << "Calling yylex()..." << std::endl;
    int token_t = yylex();  // Получаем код токена от Flex
    std::cout << "yylex() returned: " << token_t << std::endl;
    
    TokenType tokenType = static_cast<TokenType>(token_t); // Преобразуем код в тип токена
    // Вручную создаем структуру Token
    return Token(
      tokenType,              // код токена
      yytext,                 // текст из Flex
      yylineno                // номер строки
    );
  }
};

#endif