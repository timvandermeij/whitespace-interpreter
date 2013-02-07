#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <string>
#include <fstream>
#include <exception>

using namespace std;

class UnreachableToken: public exception {
    virtual const char *what() const throw () {
        return "Error: unreachable token.";
    }
} unreachableToken;

class NoArgument: public exception {
};

class NoNumericArgument: public NoArgument {
    virtual const char *what() const throw () {
        return "Error: no numeric argument was passed.";
    }
} noNumericArg;

class NoLabelArgument: public NoArgument {
    virtual const char *what() const throw () {
        return "Error: no label argument was passed.";
    }
} noLabelArg;

class prematureEndException: public exception {
    virtual const char *what() const throw () {
        return "Error: number or label ended prematurely.";
    }
} prematureEndException;

class undefinedSignException: public exception {
    virtual const char *what() const throw () {
        return "Error: sign of the number is undefined.";
    }
} undefinedSignException;

enum Token {
    LINEFEED, SPACE, TAB
};

enum Mode {
    STACKMANIP, ARITH, HEAPACC, FLOWCONT, IO
};

enum Instruction {
    PUSH, DUP, COPY, SWAP, DISCARD, SLIDE, // Stack manipulations
    ADD, SUB, MUL, DIV, MOD, // Arithmetic operations
    STORE, RETRIEVE, // Heap access
    MARK, CALL, JUMP, JUMPZERO, JUMPNEG, ENDSUB, ENDPROG, // Flow control
    WRITEC, WRITEN, READC, READN // I/O operations
};

enum Signs {
    POSITIVE, NEGATIVE
};

typedef vector<Instruction> Program;

class Interpreter {
    public:

    private:
        vector<Instruction> program; // Contains instructions from the Whitespace source
        vector<int> heap;
        vector<int> stack; // To store values
        vector<int> callStack; // To remember where to return to
        unsigned pc; // Program counter
};

vector<Token> tokenise(const string &program) {
    vector<Token> tokens;

    for(auto k = program.begin(); k != program.end(); k++) {
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

    for(int k = 0; k < length; k++) {
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
        if(length != k + 1) {
            cout << ":";
        }
    }
}

const string readFile(const string filename) {
    string line, fileContents;
    ifstream input;

    input.open(filename.c_str());
    if (input.is_open()) {
        while (!input.eof()) {
            getline(input, line);
            fileContents.append(line);
            fileContents.append("\n");
        }
    }
    return fileContents;
}

// Note: could be implemented using some sort of lookup table
//       data structure indexed by a pair of enums.
const Mode determineMode(const Token t1, const Token t2) {
    switch(t1) {
        case LINEFEED:
            return FLOWCONT;
        case SPACE:
            return STACKMANIP;
        case TAB:
            switch(t2) {
                case LINEFEED:
                    return IO;
                case SPACE:
                    return ARITH;
                case TAB:
                    return HEAPACC;
                default:
                    throw unreachableToken;
            }
            break;
        default:
            throw unreachableToken;
            break;
    }
}

const Instruction determineInstruction(const Mode m, const Token t1, const Token t2) {

}

// Side-effect: mutates the index from the for-loop in tokensToProgram
long tokensToNumber(const vector<Token> &tokens, int &index) {
    vector<Token> binNum;
    int amount = tokens.size();
    int sign;

    while(tokens[index] != LINEFEED) { // A number is terminated by a LINEFEED
        binNum.push_back(tokens[index++]);
        if(index == amount) {
            throw prematureEndException;
        }
    }

    if(binNum.front() == SPACE) {
        sign = POSITIVE;
    } else if(binNum.front() == TAB) {
        sign = NEGATIVE;
    } else {
        throw undefinedSignException;
    }
    return 1; // This should be the binary number itself. Keep in mind that the first item in the vector is the sign bit.
}

// Side-effect: mutates the index from the for-loop in tokensToProgram
vector<Token> tokensToLabel(const vector<Token> &tokens, int &index) {
    vector<Token> label;
    int amount = tokens.size();

    while(tokens[index] != LINEFEED) { // A label is terminated by a LINEFEED
        label.push_back(tokens[index++]);
        if(index == amount) {
            throw prematureEndException;
        }
    }
    return label;
}

void processStackManip(const vector<Token> &tokens, Program &p, int &k) {
    if(tokens[k] == SPACE) { // PUSH
        p.push_back(PUSH);
        if(tokens[++k] == LINEFEED) { // No number as argument to PUSH
            throw noNumericArg;
        } else { // We're going to parse the number now
            p.push_back(tokensToNumber(tokens, k));
        }
    } else if(tokens[k] == TAB) { // COPY or SLIDE
        k++;
        if(tokens[k] == SPACE) { // COPY
            p.push_back(COPY);
        } else if(tokens[k] == LINEFEED) { // COPY
            p.push_back(SLIDE);
        } else {
            throw unreachableToken;
        }
        // COPY and SLIDE both require a numeric argument,
        // so we shall try to parse a number now
        if(tokens[++k] == LINEFEED) { // No number as argument to COPY or SLIDE
            throw noNumericArg;
        } else { // The actual parsing is situated in this branch
            p.push_back(tokensToNumber(tokens, k));
        } // Duplicated from PUSH
    } else if(tokens[k] == LINEFEED) { // DUP, SWAP or DISCARD
        // Could perhaps also be done more concisely
        // with the ?:-operator, but I reckon something will go wrong
        // in the exception branch
        k++;
        if(tokens[k] == SPACE) { // DUP
            p.push_back(DUP);
        } else if(tokens[k] == TAB) { // SWAP
            p.push_back(SWAP);
        } else if(tokens[k] == LINEFEED) { // DISCARD
            p.push_back(DISCARD);
        } else {
            throw unreachableToken;
        }
    } else {
        throw unreachableToken;
    }
}

void processArith(const vector<Token> &tokens, Program &p, int &k) {
    if(tokens[k] == SPACE) {
        k++;
        if(tokens[k] == SPACE) { // ADD
            p.push_back(ADD);
        } else if(tokens[k] == TAB) { // SUB
            p.push_back(SUB);
        } else if(tokens[k] == LINEFEED) { // MUL
            p.push_back(MUL);
        } else {
            throw unreachableToken;
        }
    } else if(tokens[k] == TAB) {
        k++;
        if(tokens[k] == SPACE) { // DIV
            p.push_back(DIV);
        } else if(tokens[k] == TAB) { // MOD
            p.push_back(MOD);
        } else {
            throw unreachableToken;
        }
    } else {
        throw unreachableToken;
    }
}

void processHeapAcc(const vector<Token> &tokens, Program &p, int &k) {
    if(tokens[k] == SPACE) { // STORE
        p.push_back(STORE);
    } else if(tokens[k] == TAB) { // RETRIEVE
        p.push_back(RETRIEVE);
    } else {
        throw unreachableToken;
    }
}

void processFlowCont(const vector<Token> &tokens, Program &p, int &k) {
    if(tokens[k] == SPACE) {
        k++
        if(tokens[k] == SPACE) { // MARK
            p.push_back(MARK);
            if(tokens[++k] == LINEFEED) { // No label as argument to MARK
                throw noLabelArg;
            } else { // We're going to parse the label now
                p.push_back(tokensToNumber(tokens, k));
            }
        } else if(tokens[k] == TAB) { // CALL
            p.push_back(CALL);
            if(tokens[++k] == LINEFEED) { // No label as argument to CALL
                throw noLabelArg;
            } else { // We're going to parse the label now
                p.push_back(tokensToNumber(tokens, k));
            }
        } else if(tokens[k] == LINEFEED) { // JUMP
            p.push_back(JUMP);
            if(tokens[++k] == LINEFEED) { // No label as argument to JUMP
                throw noLabelArg;
            } else { // We're going to parse the label now
                p.push_back(tokensToNumber(tokens, k));
            }
        } else {
            throw unreachableToken;
        }
    } else if(tokens[k] == TAB) {
        k++;
        if(tokens[k] == SPACE) { // JUMPZERO
            p.push_back(JUMPZERO);
            if(tokens[++k] == LINEFEED) { // No label as argument to JUMPZERO
                throw noLabelArg;
            } else { // We're going to parse the label now
                p.push_back(tokensToNumber(tokens, k));
            }
        } else if(tokens[k] == TAB) { // JUMPNEG
            p.push_back(JUMPNEG);
            if(tokens[++k] == LINEFEED) { // No label as argument to JUMPNEG
                throw noLabelArg;
            } else { // We're going to parse the label now
                p.push_back(tokensToNumber(tokens, k));
            }
        } else if(tokens[k] == LINEFEED) { // ENDSUB
            p.push_back(ENDSUB);
        } else {
            throw unreachableToken;
        }
    } else if(tokens[k++] == LINEFEED && tokens[k] == LINEFEED) { // ENDPROG
        p.push_back(ENDPROG);
    } else {
        throw unreachableToken;
    }
}

void processIO(const vector<Token> &tokens, Program &p, int &k) {
    if(tokens[k] == SPACE) {
        k++;
        if(tokens[k] == SPACE) { // WRITEC
            p.push_back(WRITEC);
        } else if(tokens[k] == TAB) { // WRITEN
            p.push_back(WRITEN);
        } else {
            throw unreachableToken;
        }
    } else if(tokens[k] == TAB) {
        k++;
        if(tokens[k] == SPACE) { // READC
            p.push_back(READC);
        } else if(tokens[k] == TAB) { // READN
            p.push_back(READN);
        } else {
            throw unreachableToken;
        }
    } else {
        throw unreachableToken;
    }
}

// Let us first complete this potentially monolithic function and refactor it afterwards.
Program tokensToProgram(const vector<Token> &tokens) {
    int amount = tokens.size();
    if(amount < 3) {
        exit(0); // Empty program
    }

    Mode m;
    try {
        m = determineMode(tokens[0], tokens[1]);
    } catch (exception& e) { // Unreachable token exception
        throw e;
    }
    Program p;

    // FLOWCONT and STACKMANIP are only 1 token long, the rest is 2 tokens long
    int start = ((m == FLOWCONT || m == STACKMANIP) ? 1 : 2);

    for(int k = start; k < amount; k++) {
        if(m == STACKMANIP) {
            processStackManip(tokens, p, k);
        } else if(m == ARITH) {
            processArith(tokens, p, k);
        } else if(m == HEAPACC) {
            processHeapAcc(tokens, p, k);
        } else if(m == FLOWCONT) {
            processFlowCont(tokens, p, k);
        } else if(m == IO) {
            processIO(tokens, p, k);
        } else {
            throw unreachableToken;
        }
    }
    return p;
}

int main() {
    string fileContents = readFile("hello_world.ws");
    printTokens(tokenise(fileContents));
    cout << endl;
    return 0;
}
