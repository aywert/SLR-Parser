#include <iostream>
#include <stack>
#include "Grammar.hpp"
#include <map>

class SyntaxAnalyzer final {
  const Grammar& grammar;
  std::vector<std::set<Item>> states;
  std::map<std::pair<int, Symbol>, int> transitions; // state transition table: (current_state, symbol) -> next_state

  // Таблицы для парсинга
  std::map<std::pair<int, Symbol>, std::string> actionTable;
  std::map<std::pair<int, Symbol>, int> gotoTable;
  
  // Стек парсера
  std::stack<int> stateStack;

  public: 
    SyntaxAnalyzer(const Grammar& g) : grammar(g) {}
    void BuildAutomation();
    int findStateIndex(const std::set<Item>& state) const;
    void printStates() const;
};