#include <stdexcept>
#include "SLRtableGen.hpp"
#include "lexer.hpp"

constexpr const char* parse_latex_file = "parse";
constexpr const char* parse_latex_dir  = "output";

struct StackItem {
  int state_;        
  std::string symbol_;

  StackItem(int state, std::string symbol): 
    state_(state), symbol_(symbol) {}; 
};

enum class ActionType { Accept, Reduce, Shift, Mistake};

struct Action {
  ActionType type;
  int number;
};

struct hashForTest {
  std::size_t operator()(const std::vector<std::vector<std::string>>& data) const {
    std::size_t seed = data.size();
    for (const auto& row : data) {
      for (const auto& elem: row) {
        // Используем std::hash для строки
        std::hash<std::string> hasher;
        // Комбинируем хэш текущей строки с общим хэшем
        seed ^= hasher(elem) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
      }
    }
    return seed;
  }
};

class Parser {
 std::stack<StackItem> stack_; // хранит состояние и текущий символ

  MyLexer& lexer_;                  
  SLRtableGen& SLR_tg_;
  std::vector<std::vector<std::string>> data_; // data for LatexDump

  public:
    Parser(MyLexer& lexer, SLRtableGen& SLR_tg): 
      lexer_(lexer), 
      SLR_tg_(SLR_tg) {};

    void execParse();

    struct Action findAction(int currentState, std::string& currentToken) const;

    void LatexDump(const char* const  file_name, const char* const  dir_name) const;

    void printRow(const std::string& stack, 
                  const std::string& input, 
                  const std::string& action) const;

    std::string getStackString() const;
    std::string getInputString(int pos, const std::vector<Token>& tokens) const;
    std::string getRuleString(int ruleNum) const ;

    size_t GetHash() {
      struct hashForTest hasher;
      return hasher(data_);
    };
};