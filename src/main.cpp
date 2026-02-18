#include <iostream>
#include <FlexLexer.h>
#include <fstream>
#include "lexer.hpp"

int main(void) {

  std::cout << "before failing\n";
  std::ifstream file("test.txt");  // Открываем файл для чтения
    
  if (!file.is_open()) {
    std::cerr << "Sorry Couldn't open the file" << std::endl;
    return 1;
  }

  MyLexer lexer(&file);

  Token t = lexer.getNextToken(); 
  while (t.check()) {
    t.Dump();
    t = lexer.getNextToken();
  }

  return 0;
}