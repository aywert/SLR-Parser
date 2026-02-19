#include <iostream>
#include <FlexLexer.h>
#include <fstream>
#include "lexer.hpp"
#include "SLR_gen.hpp"

int main(void) {

  std::ifstream file("test.txt");  // Открываем файл для чтения
    
  if (!file.is_open()) {
    std::cerr << "Sorry couldn't open the file" << std::endl;
    return 1;
  }

  MyLexer lexer(&file);

  Token t = lexer.getNextToken(); 
  while (t.check()) {
    t.Dump();
    t = lexer.getNextToken();
  }

  Grammar gr;
  std::vector<std::vector<Symbol>> v = gr.getProductions({SymbolType::NON_TERMINAL, "E"});

  for (auto it = v.begin(); it != v.end(); ++it) {
    for (auto it_2 = it->begin(); it_2 != it->end(); ++it_2) {
      std::cout << it_2->name_ << " ";
    }

    std::cout << std::endl;
  }

  return 0;
}