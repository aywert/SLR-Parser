
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
