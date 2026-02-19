#include <iostream>
#include <stack>
#include "lexer.hpp"
 

class SyntaxAnalyzer final {
  std::stack<Token> st_;

  public: 

    SyntaxAnalyzer() = default;
    void shift();
    void reduce();
    void accept();
    void error();
};