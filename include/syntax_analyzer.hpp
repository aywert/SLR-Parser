#include <iostream>
#include <fstream>
#include <stack>
#include <map>
#include <cstdlib>
#include "Grammar.hpp"


constexpr const char* latex_file = "./output/latex.tex";
constexpr const char* latex_dir  = "output";

class SyntaxAnalyzer final {
  const Grammar& grammar;
  std::vector<std::set<Item>> states;
  std::vector<Item> grammarRules; // as i don't have strict order of rules in Grammar class, 
                                  //i will create my own vector of rules to be able to refer to them by index  

  // Таблицы для парсинга
  std::map<std::pair<int, Symbol>, std::string> actionTable;
  std::map<std::pair<int, Symbol>, int> gotoTable;
  
  // Стек парсера
  std::stack<int> stateStack;

  public: 
    SyntaxAnalyzer(const Grammar& g);
    size_t getGrammarRuleIndex(const Item& item) const;
    void BuildAutomation();
    int findStateIndex(const std::set<Item>& state) const;
   
    void printTables() const;
    void LatexDump(const char* const  file_name, const char* const dir_name) const;
    void printStates() const;
    void createSLRTable();
};


class LatexCreater {
  std::ofstream file_;
  std::string filename_;
  std::string dir_name_;
  public : 
    LatexCreater(const char* const file_name, const char* const dir_name) : filename_(file_name), dir_name_(dir_name) {
      file_.open(file_name);  // Открываем файл для чтения
      if (!file_.is_open()) {
        std::cerr << "Sorry couldn't open the file" << std::endl;
        return;
      }
    }

    void addHeader(const std::string& title, const std::string& author) {
      file_ << R"(\documentclass{article}
                \usepackage[utf8]{inputenc})";
      file_ << "\\title{" << title << "}\n";
      file_ << "\\author{" << author << "}\n";
      file_ << "\\begin{document}\n";
      file_ << "\\maketitle\n\n";
    }

    void addTable(const std::vector<std::vector<std::string>>& data) {
        file_ << "\\begin{tabular}{|";
        for (size_t i = 0; i < data[0].size(); ++i) file_ << "c|";
        file_ << "}\n\\hline\n";
        
        for (const auto& row : data) {
          for (size_t i = 0; i < row.size(); ++i) {
            if (row[i] == "$") file_ << "\\$";
            else
              file_ << row[i];
            if (i < row.size() - 1) file_ << " & ";
          }
          file_ << " \\\\ \\hline\n";
        }
        file_ << "\\end{tabular}\n\n";
    }

    void close() {
      if (file_.is_open()) {
        file_.flush();  // сбросить буферы
        file_.close();  // закрыть файл
      }
    }

    void finish() {
      file_ << "\\end{document}\n";
      close();
    }

    

    std::string getFilename() const { return filename_; }
    std::string getDirName() const { return dir_name_; }
};
  