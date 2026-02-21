#include <iostream>
#include <FlexLexer.h>
#include <fstream>
#include "lexer.hpp"
#include "Grammar.hpp"
#include "syntax_analyzer.hpp"

int main(void) {

  std::ifstream file("test.txt");  // Открываем файл для чтения
    
  if (!file.is_open()) {
    std::cerr << "Sorry couldn't open the file" << std::endl;
    return 1;
  }

  MyLexer lexer(&file);

  Token t = lexer.getNextToken(); 
  while (t.check()) {
    //t.Dump();
    t = lexer.getNextToken();
  }

  Grammar gr;
  // std::vector<std::vector<Symbol>> v = gr.getProductions({SymbolType::NON_TERMINAL, "E"});

  // for (auto it = v.begin(); it != v.end(); ++it) {
  //   for (auto it_2 = it->begin(); it_2 != it->end(); ++it_2) {
  //     std::cout << it_2->name_ << " ";
  //   }

  //   std::cout << std::endl;
  // }

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
  Item I(E, {{E, Minus, T}}, 0);

  // std::set<Item> set = gr.Closure({I});
  // std::set<Item> set2 = gr.Goto(set, E);

  // printf("Closure\n");
  // for (const auto& element : set) {
  //   element.printItem();
  // }
  // printf("Goto\n");
  // for (const auto& element : set2) {
  //   element.printItem();
  // }

  // std::cout << "FOLLOW: ";
  // std::set<Symbol> set3 = gr.Follow(T);
  // for (const auto& element : set3) {
  //   std::cout << element.name_ << " ";
  // }

  SyntaxAnalyzer sa(gr);
  sa.BuildAutomation();

  // Symbol after = I.getSymbolAfterDot();
  // std::cout << after.name_;

  return 0;
}