#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <string>
#include <fstream>

using namespace std;

enum Token {
    LINEFEED,
    SPACE,
    TAB,
};

enum Mode {
  STACK,
  ARITH,
  HEAP,
  FLOWCONT,
  IO,
  NOTHING,
};

enum Instruction {
    PUSH, DUP, COPY, SWAP, DISCARD, SLIDE, // stack manipulations
    ADD, SUB, MUL, DIV, MOD, // arithmetic operations
    STORE, RETRIEVE, // heap access
    MARK, CALL, JUMP, JUMPZERO, JUMPNEG, ENDSUB, ENDPROG, // flow control
    WRITEC, WRITEN, READC, READN, // i/o operations
};

typedef vector<Instruction> Program;

class Interpreter {
  public:
  private:
    vector<Instruction> program;
    vector<int> heap;
    vector<int> stack; //
    vector<int> callStack; // to remember where to return to
    unsigned int pc; // program counter
};

vector<Token> tokenise(const string &program) {
  vector<Token> tokens;

  for(auto k = program.begin(); k != program.end(); ++k) {
    switch(*k) {
      case '\n':
	    tokens.push_back(LINEFEED);
	break;
      case ' ':
	    tokens.push_back(SPACE);
	break;
      case '\t':
	    tokens.push_back(TAB);
	break;
    }
  }
  return tokens;
}

void printTokens(const vector<Token> &tokens) {
  int length = tokens.size();
  for(int k = 0; k < length; ++k) {
    switch(tokens[k]) {
      case LINEFEED:
	cout << "LF";
	break;
      case SPACE:
	cout << "S";
	break;
      case TAB:
	cout << "T";
	break;
    }
    if(k + 1 != length)
      cout << ":";
  }
}

const string readFile(const string filename) {
    string line, fileContents;
    ifstream input;

    input.open(filename.c_str());
    if(input.is_open()) {
        while(!input.eof()) {
            getline(input, line);
            fileContents.append(line);
            fileContents.append("\n");
        }
    }
    return fileContents;
}

// note: could be implemented using some sort of lookup table
//       data structure indexed by a pair of enums.
const Mode determineMode(const Token t1, const Token t2) {
  switch(t1) {
    case LINEFEED:
      return FLOWCONT;
    case SPACE:
      return STACK;
    case TAB:
      switch(t2) {
        case LINEFEED:
	      return IO;
        case SPACE:
	      return ARITH;
        case TAB:
	      return HEAP;
        default:
	      return NOTHING;
      }
    default:
      return NOTHING;
  }
}

Program tokensToProgram(const vector<Token> &tokens) {
  int amount = tokens.size();
  if(amount < 3) exit(0); // empty program
  Mode m = determineMode(tokens[1], tokens[2]);
  if(m == NOTHING) {
    cerr << "Error: unreachable token" << endl;
    exit(1);
  }
  if(m == FLOWCONT) { // case [LF][LF][LF] means: exit the program. Not sure if here is the right place, as more rules will come...
      if(tokens[2] == LINEFEED && tokens[3] == LINEFEED) {
          exit(0);
      }
  }
  Program p;
  return p;
}

int main() {
  string fileContents = readFile("hello_world.ws");
  printTokens(tokenise(fileContents));
  cout << endl;
  return 0;
}
