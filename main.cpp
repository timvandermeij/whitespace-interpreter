#include <iostream>
#include <list>
#include <algorithm>

using namespace std;

enum Token {
  LINEFEED,
  SPACE,
  TAB,
};

enum Instruction {
  PUSH, DUP, COPY, SWAP, DISCARD, SLIDE, // stack manipulations
  ADD, SUB, MUL, DIV, MOD, // arithmetic operations
  STORE, RETRIEVE, // heap access
  MARK, CALL, JUMP, JUMPZERO, JUMPNEG, ENDSUB, ENDPROG, // flow control
  WRITEC, WRITEN, READC, READN, // i/o operations
};

class Interpreter {
  public:
  private:
    list<Instruction> program;
    list<int> heap;
    list<int> stack; //
    list<int> callStack; // to remember where to return to
    unsigned int pc; // program counter
};

list<Token> tokenise(const string &program) {
  list<Token> tokens;

  for(auto k = program.begin(); k != program.end(); ++k) {
    switch(*k) {
      case '\n': tokens.push_back(LINEFEED); break;
      case ' ': tokens.push_back(SPACE); break;
      case '\t': tokens.push_back(TAB); break;
    }
  }
  
  return tokens;
}

void printTokens(list<Token> tokens) {
  int length = tokens.size();
  for(int k = 0; k < length; ++k) {
    switch(tokens.front()) {
      case LINEFEED: cout << "LF"; break;
      case SPACE: cout << "S"; break;
      case TAB: cout << "T"; break;
    }
    tokens.pop_front();
    if(k + 1 != length)
      cout << ":";
  }
}

int main() {
    cout << "Hello whitespace" << endl;
    //Instruction a = 500;
    string test("\t\n \n\t   ");
    printTokens(tokenise(test));
    cout << endl;
    return 0;
}
