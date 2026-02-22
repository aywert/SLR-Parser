#include <iostream>
#include <fstream>
#include <stack>
#include <map>
#include <cstdlib>
#include "Grammar.hpp"


constexpr const char* slr_latex_file = "SLRtable";
constexpr const char* slr_latex_dir  = "output";

class SLRtableGen {
  const Grammar& grammar;
  std::vector<std::set<Item>> states;
    
  std::vector<Symbol> grammarTerminals; 
  std::vector<Symbol> grammarNonTerminals; 

   
  public: 
    // Tables for parsing (creating them open as they're demanded for class Parser)
    std::map<std::pair<int, Symbol>, std::string> actionTable;
    std::map<std::pair<int, Symbol>, int> gotoTable;

    std::vector<Item> grammarRules; // as i don't have strict order of rules in Grammar class, 
                                  //i will create my own vector of rules to be able to refer to them by index 

    SLRtableGen(const Grammar& g);
    void createSLRTable();

    size_t getGrammarRuleIndex(const Item& item) const;

    void BuildAutomation();
    int findStateIndex(const std::set<Item>& state) const;
    std::vector<Symbol> getGrTerminals() const { return grammarTerminals; }
    std::vector<Symbol> getGrNonTerminals() const { return grammarNonTerminals; }

    void printTables() const;
    void LatexDump(const char* const  file_name, const char* const dir_name) const;
    void printStates() const;
    
};


class LatexCreater {
  std::ofstream file_;
  std::string file_name_;
  std::string dir_name_;
  public : 
    LatexCreater(const char* const file_name, const char* const dir_name) : file_name_(file_name), dir_name_(dir_name) {
      file_.open(dir_name_ + "/" + file_name_ + ".tex");  // Открываем файл для чтения
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

    void addTable(const std::vector<std::vector<std::string>>& data, const SLRtableGen* sa) {
      file_ << "\\begin{tabular}{|";
      for (size_t i = 0; i < data[0].size(); ++i) file_ << "c|";
      file_ << "}\n\\hline\n";

      // Первая строка заголовка: "Состояние", "ACTION", "GOTO"
      file_ << "State"; 
    
      // ACTION занимает количество терминалов столбцов
      if (sa->getGrTerminals().size() > 0) {
        file_ << " & \\multicolumn{" << sa->getGrTerminals().size() << "}{|c|}{ACTION}";
      }
    
    // GOTO занимает количество нетерминалов столбцов
    if (!sa->getGrNonTerminals().empty()) {
        file_ << " & \\multicolumn{" << sa->getGrNonTerminals().size() << "}{|c|}{GOTO}";
    }
    
    file_ << " \\\\\n";
    file_ << "\\hline\n";
    
    // Вторая строка заголовка: имена терминалов и нетерминалов
    file_ << " & ";

    for (size_t i = 0; i < sa->getGrTerminals().size(); ++i) {
      if (sa->getGrTerminals()[i].name_ == "$") file_ << " (\\$)";
      else file_ << sa->getGrTerminals()[i].name_;
      if (i != sa->getGrTerminals().size() - 1) file_ << " & ";
    }

    file_ << " & ";

    for (size_t i = 0; i < sa->getGrNonTerminals().size(); ++i) {
      
      file_ << sa->getGrNonTerminals()[i].name_;
      if (i != sa->getGrNonTerminals().size() - 1) file_ << " & ";
    }

    file_ << " \\\\\n";
    file_ << "\\hline\n";
    // Данные таблицы
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

    void addTable(const std::vector<std::vector<std::string>>& data) {
      file_ << "\\begin{tabular}{|";
      for (size_t i = 0; i < 3; ++i) file_ << "c|";
      file_ << "}\n\\hline\n";

      file_ << "STACK & INPUT & ACTION \\\\\n"; 
      file_ << "\\hline\n";                    

      for (const auto& row : data) {
        for (size_t i = 0; i < row.size(); ++i) {
          if (i == 0) file_ << "\\" << row[i];
          if (i == 1) file_ << row[i] << "$$$\\$";
          if (i == 2) file_ << row[i];

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

    

    std::string getFilename() const { return file_name_; }
    std::string getDirName() const { return dir_name_; }
};
  