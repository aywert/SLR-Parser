#include <iostream>
#include <FlexLexer.h>
#include <fstream>
// #include "lexer.hpp"
// #include "Grammar.hpp"
// #include "SLRtableGen.hpp"
#include <sstream>
#include <iostream>
#include "Parser.hpp"

int main(int argc, char** argv) {

  if(argc != 2) return 0;
  
  std::string inputfile_name = argv[1];
  std::ifstream file(inputfile_name);
    
  if (!file.is_open()) {
    std::cout <<  inputfile_name << "\n";
    std::cerr << "Sorry couldn't open the file" << std::endl;
    return 1;
  }

  MyLexer lexer(&file);

  Grammar gr;
  SLRtableGen sa(gr);

  sa.BuildAutomation();
  sa.createSLRTable();
  
  Parser prs(lexer, sa);
  prs.execParse();

  #ifdef PDF_CREATE
  use LatexDumps only from the root of the project like this: ./build/SLR input.txt
  sa.LatexDump(slr_latex_file, slr_latex_dir);
  prs.LatexDump(parse_latex_file, parse_latex_dir);
  #endif


  size_t hash = prs.GetHash();
  std::cout << hash;
}