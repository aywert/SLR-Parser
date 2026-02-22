#include <iostream>
#include <stack>
#include "Grammar.hpp"
#include <map>

class SyntaxAnalyzer final {
  const Grammar& grammar;
  std::vector<std::set<Item>> states;
  std::vector<Item> grammarRules; // as i don't have strict order of rules in Grammar class, 
                                  //i will create my own vector of rules to be able to refer to them by index  


  //std::map<std::pair<int, Symbol>, int> transitions; // state transition table: (current_state, symbol) -> next_state

  // Таблицы для парсинга
  std::map<std::pair<int, Symbol>, std::string> actionTable;
  std::map<std::pair<int, Symbol>, int> gotoTable;
  
  // Стек парсера
  std::stack<int> stateStack;

  public: 
    SyntaxAnalyzer(const Grammar& g);
    size_t getGrammarRuleIndex(const Item& item) const;
    void BuildAutomation();
    int findStateIndex(const std::set<Item>& state) const;
   
    void printTables() const;
    void printStates() const;
    void createSLRTable();
};


  