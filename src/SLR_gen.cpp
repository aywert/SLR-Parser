
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
