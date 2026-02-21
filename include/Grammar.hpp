#ifndef GRAMMAR_HPP
#define GRAMMAR_HPP

#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <unordered_map>
#include <set>

enum class SymbolType {
  TERMINAL,
  NON_TERMINAL
};

struct Symbol {
  SymbolType type_; //TERMINAL/NON_TERMINAL
  std::string name_; // F/T/id and so on

  bool operator==(const Symbol& other) const { 
    return type_ == other.type_ && name_ == other.name_;
  } // For unordered_map
   
  bool operator<(const Symbol& other) const { // For std::set
    //if (type_ != other.type_) return type_ < other.type_;
    return name_ < other.name_;
  }

  Symbol() = default;
  Symbol(SymbolType type, const std::string& name) : type_(type), name_(name) {};
  Symbol(const Symbol& sym) : type_(sym.type_), name_(sym.name_) {};
  Symbol(const Symbol&& sym) : type_(sym.type_), name_(sym.name_) {};
  Symbol& operator=(const Symbol& sym) {
    this->type_ = sym.type_;
    this->name_ = sym.name_;
    return *this;
  }
};

// struct SymbolHash {
//   size_t operator()(const Symbol& s) const {
//     return std::hash<std::string>{}(s.name_);
//   }
// };

struct SymbolHash {
  size_t operator()(const Symbol& s) const {
    size_t h1 = std::hash<int>{}(static_cast<int>(s.type_));
    size_t h2 = std::hash<std::string>{}(s.name_);
    return h1 ^ (h2 << 1);
  }
};

struct Item {
  Symbol left_;
  std::vector<Symbol> right_;
  int dot_pos_;

  bool operator==(const Item& other) const { 
    return (left_ == other.left_) && (right_ == other.right_) && (dot_pos_ == other.dot_pos_); 
  }

  bool operator<(const Item& other) const {
    if (left_ < other.left_) return true;
    if (other.left_ < left_) return false;
    
    if (dot_pos_ < other.dot_pos_) return true;
    if (other.dot_pos_ < dot_pos_) return false;
    
    return right_ < other.right_;
  };

  Item(const Item& I) : 
        left_(I.left_), right_(I.right_), dot_pos_(I.dot_pos_) {};

  Item(const Symbol& left, const std::vector<Symbol>& right, int dot_pos) : 
        left_(left), right_(right), dot_pos_(dot_pos) {};

  Symbol getSymbolAfterDot() const {
    if (dot_pos_ < right_.size()) {
      return right_[dot_pos_];
    }
    printf("oMGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGg\n\n\n");
    return {SymbolType::TERMINAL, ""}; // empty symbol if dot is at the end
  }

  void printItem() const {
    std::cout << left_.name_ << " -> ";
    for (size_t i = 0; i < right_.size(); i++) {
      if (i == dot_pos_) printf("●");
      std::cout << right_[i].name_;
    }

    if (dot_pos_ == right_.size()) printf("●");

    std::cout << std::endl;
  }

  bool isComplete() const {
    return dot_pos_ == right_.size();
  }

  // move to one position
  void moveDot() {
    dot_pos_++;
  }

 
  
};

class Grammar {
  std::set<Symbol> terminals;    //set of terminal symbols 
  std::set<Symbol> nonTerminals; //set of non-terminal symbols

  std::unordered_multimap<Symbol, std::vector<Symbol>, SymbolHash> productions {}; //production: left -> right
  
  public:

  Grammar();
  Grammar(const Grammar& gr) : terminals(gr.terminals), nonTerminals(gr.nonTerminals), productions(gr.productions) {};

  std::set<Item>   Closure(const std::set<Item>& items) const;
  std::set<Item>   Goto(const std::set<Item>& items, const Symbol& smbl) const;
  std::set<Symbol> First(const Symbol& smbl) const;
  std::set<Symbol> Follow(const Symbol& smbl) const;
  //Getting productions for every NonTerm Symbols
  std::vector<std::vector<Symbol>> getProductions(const Symbol& nonTerm) const;
  std::set<Symbol> getAllSymbols() const;
  void printProduction(std::vector<std::vector<Symbol>>& lst) const;
};

#endif