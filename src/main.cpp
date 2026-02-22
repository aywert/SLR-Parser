#include <iostream>
#include <FlexLexer.h>
#include <fstream>
// #include "lexer.hpp"
// #include "Grammar.hpp"
// #include "SLRtableGen.hpp"
#include "Parser.hpp"

int main(void) {

  std::ifstream file("input.txt");  // Открываем файл для чтения
    
  if (!file.is_open()) {
    std::cerr << "Sorry couldn't open the file" << std::endl;
    return 1;
  }

  MyLexer lexer(&file);

  Grammar gr;
  SLRtableGen sa(gr);

  sa.BuildAutomation();
  sa.createSLRTable();
  sa.LatexDump(slr_latex_file, slr_latex_dir);
  //lexer.showTokens();
  Parser prs(lexer, sa);
  prs.execParse();
  prs.LatexDump(parse_latex_file, parse_latex_dir);
  
  // Symbol after = I.getSymbolAfterDot();
  // std::cout << after.name_;

  return 0;
}