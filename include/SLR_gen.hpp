#ifndef SLR_GEN_HPP
#define SLR_GEN_HPP

#include <iostream>
#include <vector>
#include <unordered_map>
#include <set>

enum class SymbolType {
  TERMINAL,
  NON_TERMINAL
};

struct Symbol {
  SymbolType type_; //TERMINAL/NON_TERMINAL
  std::string name_; // F/T/id and so on

  bool operator==(const Symbol& other) const { return name_ == other.name_; } // For unordered_map
   
  bool operator<(const Symbol& other) const { // For std::set
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

struct SymbolHash {
  size_t operator()(const Symbol& s) const {
    return std::hash<std::string>{}(s.name_);
  }
};

struct Item {
  Symbol left_;
  std::vector<Symbol> right_;
  int dot_pos_;

  Symbol getSymbolAfterDot() const {
    if (dot_pos_ < right_.size()) {
      return right_[dot_pos_];
    }

    return {SymbolType::TERMINAL, ""}; // empty symbol if dot is at the end
  }

  void printItem() const {
    std::cout << left_.name_ << " -> ";
    for (size_t i = 0; i < right_.size(); i++) {
      if (i == dot_pos_) printf("#");
      std::cout << right_[i].name_;
    }

    std::cout << std::endl;
  }

  bool isComplete() const {
    return dot_pos_ == right_.size();
  }

  // move to one position
  void moveDot() {
    dot_pos_++;
  }

  bool operator<(const Item& other) const {
    return this->right_ < other.right_;
  };
  
  Item(const Item& I) : 
  left_(I.left_), right_(I.right_), dot_pos_(I.dot_pos_) {};

  Item(const Symbol& left, const std::vector<Symbol>& right, int dot_pos) : 
  left_(left), right_(right), dot_pos_(dot_pos) {};
  
};

class Grammar {
  std::set<Symbol> terminals;    //set of terminal symbols 
  std::set<Symbol> nonTerminals; //set of non-terminal symbols

  std::unordered_multimap<Symbol, std::vector<Symbol>, SymbolHash> productions {}; //production: left -> right
  
  public:

  Grammar();

  std::set<Item> Closure(const std::set<Item>& items);
  //Getting productions for every NonTerm Symbols
  std::vector<std::vector<Symbol>> getProductions(const Symbol& nonTerm) const {
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
};

#endif