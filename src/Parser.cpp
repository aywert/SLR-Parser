#include "Parser.hpp"

void Parser::execParse() {
  std::vector<Token> tokens = lexer_.getReadyBuf();
  if (tokens.size() < 1) return;

  stack_.push({0, "$"});
  // position in input buffer
  int pos = 0; 

  std::string currentToken = tokens[pos].name_;

  while (true) {
    // take state from the stack
    int currentState = stack_.top().state_; 
    
    // Looking through the actionTable map to get action to execute
    struct Action act;

    try {
      act = findAction(currentState, currentToken);
    } 
    catch (std::logic_error& err) { 
      std::cerr << "Parser error: " << err.what() << std::endl;
      break;
    };
    

    printRow(getStackString(), getInputString(pos, tokens), 
              act.type == ActionType::Shift  ? "Shift" : 
              act.type == ActionType::Reduce ? "Reduce " + getRuleString(act.number) : "acc");

    switch (act.type) {
      case ActionType::Shift: {
        int nextState = act.number;
        stack_.push(StackItem(nextState, currentToken));   
        
        // taking new token
        pos++;
        currentToken = (pos < tokens.size()) ? tokens[pos].name_ : "$";
        break;
      }

      case ActionType::Reduce: {
        // action reduce
        int ruleNum = act.number; 
        const auto& rule = SLR_tg_.grammarRules[ruleNum];
        int rightLength = rule.right_.size();
        
        // getting rid of sequence of Tokens
        for (int i = 0; i < rightLength; i++) { stack_.pop(); }

        if (stack_.empty()) {
          std::cout << "Mistake: empty stack after reduction\n";
          return;
        }
        
        int prevState = stack_.top().state_;
        
        auto gotoIt = SLR_tg_.gotoTable.find({prevState, rule.left_});
        if (gotoIt == SLR_tg_.gotoTable.end()) {
          std::cout << "Mistake: no GOTO from this state" << prevState 
                    << " for " << rule.left_.name_ << "\n";
          return;
        }
        
        int newState = gotoIt->second;
        
        stack_.push(StackItem(newState, rule.left_.name_));
        break;
      }

      case ActionType::Accept: {
        std::cout << "Ready!\n";
        return;
      }

      case ActionType::Mistake: {
        std::cerr << "Mistake while parsing\n";
        return;
      }

      default: std::cerr << " default Mistake while parsing\n"; break;
    }
  }
}

struct Action Parser::findAction(int currentState, std::string& currentToken) const {
  auto actionIt = SLR_tg_.actionTable.find({currentState, currentToken});

  if (actionIt == SLR_tg_.actionTable.end()) {
    throw std::logic_error("no such elements");
  }
  
  std::string action = actionIt->second;

  if      (action[0] == 's') return {ActionType::Shift,  std::stoi(action.substr(1))};
  else if (action[0] == 'r') return {ActionType::Reduce, std::stoi(action.substr(1))};
  else if (action == "acc")  return {ActionType::Accept,  0};
  else                       return {ActionType::Mistake, 0};
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
