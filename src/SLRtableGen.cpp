#include "SLRtableGen.hpp"

SLRtableGen::SLRtableGen(const Grammar& g) : grammar(g) {
  for (const auto& nonterm : grammar.getNonTerminals()) {
    auto productions = grammar.getProductions(nonterm);
    for (const auto& prod : productions) {
      grammarRules.emplace_back(nonterm, prod, 0);
    }
  }

  for (const auto& term : grammar.getTerminals()) {
    grammarTerminals.push_back(term);
  }

  for (const auto& nonterm : grammar.getNonTerminals()) {
    grammarNonTerminals.push_back(nonterm);
  }
}

void SLRtableGen::BuildAutomation() {

  Symbol E_ = {SymbolType::NON_TERMINAL,  "E'"};
  Symbol E  = {SymbolType::NON_TERMINAL,  "E"};

  Item startItem(E_, {E}, 0); // E' -> .E
  states.push_back(grammar.Closure({startItem})); // first state
  size_t current = 0; //position of state we are currently processing in states vector

  while (current < states.size()) {
      const auto state = states[current];
      
      for (const auto& symbol : grammar.getAllSymbols()) {
        //std::cout << "Symbol name: " << symbol.name_ << "\n";
        auto nextState = grammar.Goto(state, symbol);
        
        if (!nextState.empty()) {
          int foundIndex = findStateIndex(nextState); //check if we already have this state in states vector
          
          if (foundIndex == -1) { // -1 means that there is no such state in states vector
            states.push_back(nextState);
            foundIndex = states.size() - 1;
            //transitions[{current, symbol}] = foundIndex;
          }  
        }
      }

    //printStates();
    current++;
  }
}

void SLRtableGen::createSLRTable() {
Symbol Dollar = {SymbolType::TERMINAL,      "$"};
Symbol E_     = {SymbolType::NON_TERMINAL,  "E'"};
// Для каждого состояния
for (size_t i = 0; i < states.size(); i++) {
  const auto state = states[i];
    
  // Для каждого пункта в состоянии
  for (const auto& item : state) {  
      
    // СЛУЧАЙ 1: Точка в конце (свертка)
    if (item.isComplete()) {
      if (item.left_ == E_) {
          // Принятие
          actionTable[{i, Dollar}] = "acc";
      } else {
        // Свертка
        int ruleNum = getGrammarRuleIndex(item);
        
        // Для всех терминалов из FOLLOW левой части
        auto followSet = grammar.Follow(item.left_);
        for (const auto& term : followSet) {
          actionTable[{i, term}] = "r" + std::to_string(ruleNum);
        }
      }
    } 
    // СЛУЧАЙ 2: Точка перед терминалом (сдвиг)
    else {
      Symbol nextSym = item.getSymbolAfterDot();
      
      if (nextSym.type_ == SymbolType::TERMINAL) {
        auto nextState = grammar.Goto(state, nextSym);
        int foundIndex = findStateIndex(nextState);
        if (foundIndex != -1) {
          actionTable[{i, nextSym}] = "s" + std::to_string(foundIndex);
        }
      }
    }
  }
  
  // Заполняем GOTO таблицу для нетерминалов
  for (const auto& nonTerm : grammar.getNonTerminals()) {

    auto nextState = grammar.Goto(state, nonTerm);
    int foundIndex = findStateIndex(nextState);
    if (foundIndex != -1) {
      gotoTable[{i, nonTerm}] = foundIndex;
    }
  }
}

}


void SLRtableGen::printTables() const {
  std::cout << "\n=== ТАБЛИЦА ACTION ===\n";
  for (const auto& [key, action] : actionTable) {
      std::cout << "ACTION[" << key.first << ", " << key.second.name_ 
                << "] = " << action << "\n";
  }
  
  std::cout << "\n=== ТАБЛИЦА GOTO ===\n";
  for (const auto& [key, state] : gotoTable) {
      std::cout << "GOTO[" << key.first << ", " << key.second.name_ 
                << "] = " << state << "\n";
  }
}

 void SLRtableGen::LatexDump(const char* const  file_name, const char* const  dir_name) const {
  LatexCreater latex(file_name, dir_name);
  latex.addHeader("SLR Parsing Tables", "Michael Movsesyan");
  std::set<Symbol> terminals = grammar.getTerminals();
  std::set<Symbol> nonTerminals = grammar.getNonTerminals();

  std::vector<std::vector<std::string>> tableData;
  // actionData.push_back({std::to_string(key.first), key.second.name_, action});
  for (size_t i = 0; i < states.size(); i++) {
    const auto state = states[i];
    std::vector<std::string> row;
    row.push_back(std::to_string(i));
   
    for (const auto& term: terminals) {
      auto it = actionTable.find({i, term});
      if (it != actionTable.end()) {
        row.push_back(it->second);
      } else {
        row.push_back("");
      }
    }

    // GOTO часть: для каждого нетерминала
    for (const auto& nonterm : nonTerminals) {
      auto it = gotoTable.find({i, nonterm});
      if (it != gotoTable.end()) {
        row.push_back(std::to_string(it->second));
      } else {
        row.push_back("");  // пустая клетка
      }
    }

    tableData.push_back(row);
  }

  latex.addTable(tableData, this);

  latex.finish();

  #ifdef PDF_CREATE
  std::string sysCallStr = "pdflatex -output-directory=" + latex.getDirName() + " -interaction=batchmode " + latex.getDirName() + "/"+ latex.getFilename() + .tex" > /dev/null ";

  std::cout << sysCallStr;
  int result = std::system(sysCallStr.c_str());

  if (result == 0) {
    std::cout << "\nCompiled succesfully" << std::endl;
    std::string openCommand = "xdg-open " + latex.getDirName() + "/" + latex.getFilename() +".pdf";
    std::system(openCommand.c_str());
  } else {
    std::cerr << "\nCompilation mistake. code: " << result << std::endl;
  }

  #endif
 }

size_t SLRtableGen::getGrammarRuleIndex(const Item& item) const {
  for (size_t i = 0; i < grammarRules.size(); i++) {
    if (grammarRules[i].left_ == item.left_ && grammarRules[i].right_ == item.right_) {
      return i;
    }
  }
  return -1; // Not found, should not happen if item is valid
}

int SLRtableGen::findStateIndex(const std::set<Item>& state) const {
  for (size_t i = 0; i < states.size(); i++) {
    if (states[i] == state) return i;
  }
  return -1;
}

void SLRtableGen::printStates() const {
  for (size_t i = 0; i < states.size(); i++) {
    std::cout << "State " << i << ":" << std::endl;
    for (const auto& item : states[i]) {
      item.printItem();
    }
    std::cout << std::endl;
  }
}
