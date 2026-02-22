#include "Parser.hpp"

void Parser::execParse() {
  std::vector<Token> tokens = lexer_.getReadyBuf();
  if (tokens.size() < 1) return;

  stack_.push({0, "$"});
  int pos = 0; // position in input buffer

  std::string currentToken = tokens[pos].name_;

  while (true) {
    int currentState = stack_.top().state_; // take state from the stack
    
    // Looking through the actionTable map to get action to execute
    auto actionIt = SLR_tg_.actionTable.find({currentState, currentToken});

    if (actionIt == SLR_tg_.actionTable.end()) {
      std::cout << "Mistake\n";
      break;
    }
    
    std::string action = actionIt->second;

    // Printing current state
    printRow(getStackString(), getInputString(pos, tokens), 
              action[0] == 's' ? "Shift" : 
              action[0] == 'r' ? "Reduce " + getRuleString(std::stoi(action.substr(1))) : 
              action);
    
    if (action[0] == 's') {
      //action shift: putting to the stack same Token with state provided in actionTable
      int nextState = std::stoi(action.substr(1));
      stack_.push(StackItem(nextState, currentToken));   
      
      // taking new token
      pos++;
      currentToken = (pos < tokens.size()) ? tokens[pos].name_ : "$";
        
    } else if (action[0] == 'r') {
        // action reduce: replacing
        int ruleNum = std::stoi(action.substr(1)); 
        const auto& rule = SLR_tg_.grammarRules[ruleNum];
        int rightLength = rule.right_.size();
        
        for (int i = 0; i < rightLength; i++) {
          stack_.pop(); // getting rid of sequence of Tokens
        }

        if (stack_.empty()) {
          std::cout << "Mistake: empty stack after reduction\n";
          break;
        }
        
        int prevState = stack_.top().state_;
        
        auto gotoIt = SLR_tg_.gotoTable.find({prevState, rule.left_});
        if (gotoIt == SLR_tg_.gotoTable.end()) {
          std::cout << "Mistake: no GOTO from this state" << prevState 
                    << " for " << rule.left_.name_ << "\n";
          break;
        }
        
        int newState = gotoIt->second;
        
        stack_.push(StackItem(newState, rule.left_.name_));
            
        } 

        else if (action == "acc") {
          std::cout << "Ready!\n";
          break;
        }
  }
}

std::string Parser::getStackString() const {
    std::stack<StackItem> temp = stack_;
    std::vector<std::string> items;
    
    while (!temp.empty()) {
        items.push_back(temp.top().symbol_);
        temp.pop();
    }
    
    std::string result;
    for (int i = items.size() - 1; i >= 0; i--) {
        result += items[i];
    }
    return result;
}

std::string Parser::getInputString(int pos, const std::vector<Token>& tokens) const {
    std::string result;
    for (size_t i = pos; i < tokens.size(); i++) {
        result += tokens[i].value_;
        if (i < tokens.size() - 1) result += " ";
    }
    return result;
}

std::string Parser::getRuleString(int ruleNum) const {
    const auto& rule = SLR_tg_.grammarRules[ruleNum];
    std::string result = rule.left_.name_ + " -> ";
    for (const auto& sym : rule.right_) {
        result += sym.name_ + " ";
    }
    return result;
}

void Parser::printRow(const std::string& stack, const std::string& input, const std::string& action) const {
    std::cout << "| " << std::left << stack
              << " | " << input
              << " | " << action << " |\n";
}
