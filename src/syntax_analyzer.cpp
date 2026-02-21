#include "syntax_analyzer.hpp"

void SyntaxAnalyzer::BuildAutomation() {

  Symbol E_ = {SymbolType::NON_TERMINAL,  "E'"};
  Symbol E  = {SymbolType::NON_TERMINAL,  "E"};

  Item startItem(E_, {E}, 0); // E' -> .E

  states.push_back(grammar.Closure({startItem})); // first state

  size_t current = 0; //position of state we are currently processing in states vector

  while (current < states.size()) {
      const auto& state = states[current];
      
      for (const auto& symbol : grammar.getAllSymbols()) {
        //std::cout << "Symbol name: " << symbol.name_ << "\n";
        auto nextState = grammar.Goto(state, symbol);
        
        if (!nextState.empty()) {
          int foundIndex = findStateIndex(nextState); //check if we already have this state in states vector
          
          if (foundIndex == -1) { // -1 means that there is no such state in states vector
            states.push_back(nextState);
            // printf("States updated\n");
            // printStates();
            // printf("---");
            foundIndex = states.size() - 1;
            transitions[{current, symbol}] = foundIndex;
          }  
        }

        // else {
        //   printf("See why closure is sorry\n");
        //   //printStates();
        // }
      }
    printStates();
    current++;
  }
}

int SyntaxAnalyzer::findStateIndex(const std::set<Item>& state) const {
  for (size_t i = 0; i < states.size(); i++) {
    if (states[i] == state) return i;
  }
  return -1;
}

void SyntaxAnalyzer::printStates() const {
  for (size_t i = 0; i < states.size(); i++) {
    std::cout << "State " << i << ":" << std::endl;
    for (const auto& item : states[i]) {
      item.printItem();
    }
    std::cout << std::endl;
  }
}