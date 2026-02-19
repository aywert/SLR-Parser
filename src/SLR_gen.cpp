
#include "SLR_gen.hpp"

Grammar::Grammar() {
  Symbol E_     = {SymbolType::NON_TERMINAL,  "E'"};
  Symbol E      = {SymbolType::NON_TERMINAL,  "E"};
  Symbol T      = {SymbolType::NON_TERMINAL,  "T"};
  Symbol F      = {SymbolType::NON_TERMINAL,  "F"};
  Symbol Plus   = {SymbolType::TERMINAL,      "+"};
  Symbol Minus  = {SymbolType::TERMINAL,      "-"};
  Symbol Mul    = {SymbolType::TERMINAL,      "*"};
  Symbol Div    = {SymbolType::TERMINAL,      "/"};
  Symbol LParen = {SymbolType::TERMINAL,      "("};
  Symbol RParen = {SymbolType::TERMINAL,      ")"};
  Symbol Id     = {SymbolType::TERMINAL,      "id"};
  Symbol Dollar = {SymbolType::TERMINAL,      "$"};

   // Initialization of terminals
  terminals.emplace(Id);
  terminals.emplace(Plus);
  terminals.emplace(Minus);
  terminals.emplace(Mul);
  terminals.emplace(Div);
  terminals.emplace(LParen);
  terminals.emplace(RParen);
  terminals.emplace(Dollar);
  
  // Initialization of non-terminals
  nonTerminals.emplace(E);
  nonTerminals.emplace(T);
  nonTerminals.emplace(F);
  nonTerminals.emplace(E_); // expanded

  // Productions:
  productions.insert({E_, {E}});          // E' → E
  productions.insert({E, {E, Plus, T}});  // E → E + T
  productions.insert({E, {E, Minus, T}}); // E → E - T
  productions.insert({E, {T}});           // E → T
  productions.insert({T, {T, Mul, F}});   // T → T * F
  productions.insert({T, {T, Div, F}});   // T → T / F
  productions.insert({T, {F}});                  // T → F
  productions.insert({F, {LParen, E, RParen}});  // F → ( E )
  productions.insert({F, {Id}});                 // F → id
} 

std::set<Item> Grammar::Goto(const std::set<Item>& items, const Symbol& smbl) {
  std::set<Item> result{};
  // Проходим по каждому пункту из входного множества
  for (const Item& item : items) { // for each item
    // Проверяем: пункт не завершен И символ после точки равен искомому
    if (!item.isComplete() && item.getSymbolAfterDot() == smbl) {
      // Создаем новый пункт - копию текущего
      Item newItem = item;  \
      // Передвигаем точку на одну позицию вперед
      newItem.moveDot();
      // Добавляем в ядро
      result.insert(newItem);
    }
  }
  
  // Применяем closure к полученному множеству
  return Closure(result);

}

std::set<Item> Grammar::Closure(const std::set<Item>& items) {
  std::set<Item> result = items;  // starting with given set
  std::vector<Item> item_lst(items.begin(), items.end()); //lst contains Item yet to process;

  while (!item_lst.empty()) {
    std::vector<Item> new_item_lst; //contains items which wasn't processed
  
    for (const Item& item: item_lst) { //for each item
      if (!item.isComplete()) {
        Symbol current_smbl = item.getSymbolAfterDot(); //need to find every grammar rule which start with current_smbl
        
        if (current_smbl.type_ == SymbolType::NON_TERMINAL) {
          // Получаем все правила грамматики для этого нетерминала
          // Например, для E получим правила: E → E+T и E → T
          auto productions = getProductions(current_smbl);

          // Для каждого правила создаем новый пункт с точкой в начале
          for (const auto& prod : productions) {
            Item newItem(current_smbl, prod, 0);
            
            // Добавляем новый пункт, если его еще нет
            if (result.insert(newItem).second) {
              new_item_lst.push_back(newItem);
            }
          }
        }
      }
    }

    item_lst = std::move(new_item_lst);
  }

  return result;
}

std::vector<std::vector<Symbol>> Grammar::getProductions(const Symbol& nonTerm) const {
  std::vector<std::vector<Symbol>> result;
  if (nonTerm.type_ != SymbolType::NON_TERMINAL) {
    std::cerr << "Error: Symbol must be a non-terminal." << std::endl;
    return result;
    //throw std::invalid_argument("Symbol must be a non-terminal");
    //abstain from throwing exceptions just yet;
  }
  
  auto range = productions.equal_range(nonTerm);
  for (auto it = range.first; it != range.second; ++it) {
    result.push_back(it->second);
  }

  return result;
}

std::set<Symbol> Grammar::First(const Symbol& smbl) {
  if (smbl.type_ == SymbolType::TERMINAL) {
    return {smbl}; // First of a terminal is the terminal itself
  }

  std::set<Symbol> result = {};  // empty set for terminals
  std::set<Symbol> processed_smbl{}; // to account for every symbol processed
  std::vector<Symbol> smbl_lst = {smbl}; //lst contains Item yet to process;

  while (!smbl_lst.empty()) {
    std::vector<Symbol> new_smbl_lst; //contains items which wasn't processed

    for (const Symbol& smbl_: smbl_lst) { //for each symbol
      if (smbl_.type_ == SymbolType::TERMINAL) {
        result.insert(smbl_); // First of epsilon is epsilon itself
      } 
      else {
        if (processed_smbl.find(smbl_) == processed_smbl.end()) {
          auto range = productions.equal_range(smbl_); // getting productions for smbl_

          for (auto it = range.first; it != range.second; ++it) {        
            // Добавляем первые символы пораждаемые продукцией
            new_smbl_lst.push_back(it->second[0]);
          }

          processed_smbl.insert(smbl_);
        }
      }
    }

    smbl_lst = std::move(new_smbl_lst);
  }

  return result;
}

std::set<Symbol> Grammar::Follow(const Symbol& smbl) {
  if (smbl.type_ == SymbolType::TERMINAL) return {};
  if (smbl.name_ == std::string("E'")) {
    return {Symbol(SymbolType::TERMINAL, "$")};
  }

  std::set<Symbol> result = {}; 
  std::set<Symbol> processed_smbl{}; // to account for every symbol processed
  std::vector<Symbol> smbl_lst = {smbl};

  while(!smbl_lst.empty()) {
  std::vector<Symbol> new_smbl_lst;

    for (const Symbol& current : smbl_lst) {
      for (const auto& nTerm : nonTerminals) { //for every none terminal Symbol
        auto productions = getProductions(nTerm); //get productions

        for (const auto& prod : productions) {
          auto it = std::find(prod.begin(), prod.end(), current); //trying to find Symbol in production sequence
          if (it != prod.end()) {
            auto index = std::distance(prod.begin(), it);
            if (index != prod.size() - 1) {
              std::set<Symbol> newSet = First(prod[index+1]);
              result.insert(newSet.begin(), newSet.end());
            }
            else {
              if (nTerm.name_ == std::string("E'")) {result.insert(Symbol(SymbolType::TERMINAL, "$"));}
              if (processed_smbl.find(current) == processed_smbl.end())
                new_smbl_lst.push_back(nTerm); 
            } //adding to the list Symbols that are of FOLLOW(left)=FOLLOW(smbl) types
          }
        }
      }
      processed_smbl.insert(current);
    }

   smbl_lst = std::move(new_smbl_lst);
  }
  
  return result;
}
