#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <string>

enum TokenType {
  END_OF_FILE, //yylex return 0 when it reaches the end of the file
  IDENTIFIER,
  NUMBER,
  PLUS, 
  MINUS,
  MULTIPLY,
  LPAREN,
  RPAREN,
  ERROR
};

// Структура для хранения токена
struct Token {
  TokenType type_;
  std::string value_;
  int line_;

  public:
    Token(TokenType type, const char* value, int line) : 
    type_(type), value_(value), line_(line) {};

    Token(const Token& tkn) : 
    type_(tkn.type_), value_(tkn.value_), line_(tkn.line_) {};

    Token(Token&& tkn) : 
    type_(tkn.type_), value_(tkn.value_), line_(tkn.line_) {};

    Token& operator=(const Token& tkn) {
      this->type_ = tkn.type_;
      this->value_ = tkn.value_;
      this->line_ = tkn.line_;
      return *this;
    }

    void Dump() const {
      printf("TOKEN:\ntype: %d\nvalue: %s\nline: %d\n", type_, value_.c_str(), line_);
      printf("-------");
    }

    bool check() {
      if (type_ == END_OF_FILE) return false;
      else {

        if (type_ == ERROR) {
          value_ = "bad token";
          printf("Error token: %s at line %d\n", value_.c_str(), line_);
          return true;
        }

        else return true;
      }
    }
};


#endif