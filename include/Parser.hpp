#include "SLRtableGen.hpp"
#include "lexer.hpp"

struct StackItem {
  int state_;        
  std::string symbol_;

  StackItem(int state, std::string symbol): state_(state), symbol_(symbol) {}; 
};

class Parser {
 std::stack<StackItem> stack_; // хранит состояние и текущий символ

  MyLexer& lexer_;                  
  SLRtableGen& SLR_tg_;

  public:
    Parser(MyLexer& lexer, SLRtableGen& SLR_tg): lexer_(lexer), SLR_tg_(SLR_tg) {};
    void execParse();

    std::string getStackString() const;
    std::string getInputString(int pos, const std::vector<Token>& tokens) const;
    std::string getRuleString(int ruleNum) const ;
    void printRow(const std::string& stack, const std::string& input, const std::string& action) const;
};