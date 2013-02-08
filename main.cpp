#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <string>
#include <fstream>
#include <exception>
#include <cmath>
#include <list>
#include <map>

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

class OutOfBoundsException: public NoArgument {
    virtual const char *what() const throw () {
        return "Error: index out of bounds.";
    }
} outOfBoundsException;

class PrematureEndException: public exception {
    virtual const char *what() const throw () {
        return "Error: number or label ended prematurely.";
    }
} prematureEndException;

class UndefinedSignException: public exception {
    virtual const char *what() const throw () {
        return "Error: sign of the number is undefined.";
    }
} undefinedSignException;

class InstructionNotFoundException: public exception {
    virtual const char *what() const throw () {
        return "Error: instruction has not been found.";
    }
} instructionNotFoundException;

class LabelNotFoundException: public exception {
    virtual const char *what() const throw () {
        return "Error: label has not been found.";
    }
} labelNotFoundException;

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

typedef vector<Instruction> Program;

class Interpreter {
    public:
        Interpreter(Program);
        void interpret();

    private:
        Program p; // Contains instructions from the Whitespace source
        vector<int> heap;
        list<int> stack; // To store values
        vector<int> callStack; // To remember where to return to
        map<int, unsigned> labels; // Lookup table for labels
};

Interpreter::Interpreter(Program p) {
    this->p = p;
}

void Interpreter::interpret() {
    unsigned pc, size = p.size();

    for(pc = 0; pc < size; pc++) {
        switch(p[pc]) {
        // Stack manipulations
	    case PUSH: {
            stack.push_front(p[++pc]);
            break;
	    }
	    case DUP: {
            stack.push_front(stack.front());
            break;
	    }
	    case COPY: {
            auto it = stack.begin();
            advance(it, p[++pc]);
            stack.push_front(*it);
            break;
        }
	    case SWAP: {
            Instruction first, second;
            first = stack.front();
            stack.pop_front();
            second = stack.front();
            stack.pop_front();
            stack.push_front(first);
            stack.push_front(second);
            break;
        }
	    case DISCARD: {
            stack.pop_front();
            break;
        }
	    case SLIDE: {
            Instruction top;
            top = stack.front();
            ++pc;
            for(int i = 0; i <= p[pc]; i++) {
                stack.pop_front();
            }
            stack.push_front(top);
            break;
	    }

        // Arithmetic
	    case ADD: {
            int a = stack.front();
            stack.pop_front();
            int b = stack.front();
            stack.pop_front();
            stack.push_front(b + a);
            break;
        }
	    case SUB: {
            int a = stack.front();
            stack.pop_front();
            int b = stack.front();
            stack.pop_front();
            stack.push_front(b - a);
            break;
	    }
	    case MUL: {
            int a = stack.front();
            stack.pop_front();
            int b = stack.front();
            stack.pop_front();
            stack.push_front(b * a);
            break;
            }
	    case DIV: {
            int a = stack.front();
            stack.pop_front();
            int b = stack.front();
            stack.pop_front();
            stack.push_front(b / a);
            break;
	    }
	    case MOD: {
            int a = stack.front();
            stack.pop_front();
            int b = stack.front();
            stack.pop_front();
            stack.push_front(b % a);
            break;
        }

        // Heap access
	    case STORE: {
            int value = stack.front();
            stack.pop_front();
            int address = stack.front();
            if(address < 0) {
                throw outOfBoundsException;
            }
            int size = heap.size();
            if(size < address) {
                for(int i = size; i < address; i++) {
                    heap.push_back(0);
                }
            }
            heap.push_back(value);
            break;
        }
	    case RETRIEVE: {
            int size = heap.size();
            int address = stack.front();
            stack.pop_front();
            if((size < address) || (address < 0)) {
                throw outOfBoundsException;
            } else {
                stack.push_front(heap[address]);
            }
            break;
        }

        // Flow control
	    case MARK: {
            int label = p[++pc];
            labels.insert(pair<int, unsigned>(label, pc + 1)); // Go to next instruction after label
            break;
        }
	    case CALL: {
            int label = p[++pc];
            auto pair = labels.find(label);
            if(pair == labels.end()) { // Is this correct? Fetches last item probably, which is not what we want
                throw labelNotFoundException;
            }
            pc = pair->second;
            break;
	    }
	    case JUMP: {
	    	break;
	    }
	    case JUMPZERO: {
	    	break;
	    }
	    case JUMPNEG: {
	    	break;
	    }
	    case ENDSUB: {
	    	break;
	    }
	    case ENDPROG: {
	    	break;
	    }
	    
	    // I/O operations
	    case WRITEC: {
	    	break;
	    }
	    case WRITEN: {
	    	break;
	    }
	    case READC: {
	    	break;
	    }
	    case READN: {
	    	break;
	    }
            default:
                throw instructionNotFoundException;
        }
    }
}

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
                cout << "LF" << endl;
                break;
            case SPACE:
                cout << "S ";
                break;
            case TAB:
                cout << "T ";
                break;
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
// data structure indexed by a pair of enums.
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
        default:
            throw unreachableToken;
    }
}

// Side-effect: mutates the index from the for-loop in tokensToProgram
// Labels are also represented as numbers, so labels will be handled as well.
long tokensToNumber(const vector<Token> &tokens, int &index) {
    vector<Token> binNum;
    int amount = tokens.size();
    int sign;
    long sum = 0;

    while(tokens[index] != LINEFEED) { // A number is terminated by a LINEFEED
        binNum.push_back(tokens[index++]);
        if(index == amount) {
            throw prematureEndException;
        }
    }

    if(binNum.front() == SPACE) {
        sign = 1;
    } else if(binNum.front() == TAB) {
        sign = -1;
    } else {
        throw undefinedSignException;
    }
    binNum.erase(binNum.begin()); // Pop the sign bit
    for(int k = binNum.size() - 1; k >= 0; k++) {
    	sum += ((binNum[k] == TAB) ? pow(2, k) : 0);
    }
    return sign * sum;
}

void parseNumber(const vector<Token> &tokens, Program &p, int &k) {
    if(tokens[++k] == LINEFEED) { // No label as argument
        throw noLabelArg;
    } else { // We're going to parse the label now
        p.push_back(tokensToNumber(tokens, k));
    }
}

void processStackManip(const vector<Token> &tokens, Program &p, int &k) {
    if(tokens[k] == SPACE) { // PUSH
        p.push_back(PUSH);
        parseNumber(tokens, p, k);
    } else if(tokens[k] == TAB) {
        k++;
        if(tokens[k] == SPACE) { // COPY
            p.push_back(COPY);
        } else if(tokens[k] == LINEFEED) { // SLIDE
            p.push_back(SLIDE);
        } else {
            throw unreachableToken;
        }
        // COPY and SLIDE both require a numeric argument,
        // so we shall try to parse a number now
        parseNumber(tokens, p, k);
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
        k++;
        if(tokens[k] == SPACE) { // MARK
            p.push_back(MARK);
            parseNumber(tokens, p, k);
        } else if(tokens[k] == TAB) { // CALL
            p.push_back(CALL);
            parseNumber(tokens, p, k);
        } else if(tokens[k] == LINEFEED) { // JUMP
            p.push_back(JUMP);
            parseNumber(tokens, p, k);
        } else {
            throw unreachableToken;
        }
    } else if(tokens[k] == TAB) {
        k++;
        if(tokens[k] == SPACE) { // JUMPZERO
            p.push_back(JUMPZERO);
            parseNumber(tokens, p, k);
        } else if(tokens[k] == TAB) { // JUMPNEG
            p.push_back(JUMPNEG);
            parseNumber(tokens, p, k);
        } else if(tokens[k] == LINEFEED) { // ENDSUB
            p.push_back(ENDSUB);
        } else {
            throw unreachableToken;
        }
    } else if(tokens[k] == LINEFEED && tokens[++k] == LINEFEED) { // ENDPROG
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
        if(k == amount) {
	        cout << "k has reached max" << endl; // We might want to use an exception here...
        }

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
        k++; // Proceed to next instruction
	    if(k != amount) {
	        m = determineMode(tokens[k], tokens[k + 1]);
	        if(!(m == STACKMANIP || m == FLOWCONT)) {
                k++;
	        }
	    }
    }
    return p;
}

string programToString(Program p) {
    int size = p.size();
    string s;

    for(int k = 0; k < size; k++) {
        switch(p[k]) {
            case PUSH: s.append("PUSH "); break;
            case DUP: s.append("DUP"); break;
            case COPY: s.append("COPY "); break;
            case SWAP: s.append("SWAP"); break;
            case DISCARD: s.append("DISCARD"); break;
            case SLIDE: s.append("SLIDE "); break;
            case ADD: s.append("ADD"); break;
            case SUB: s.append("SUB"); break;
            case MUL: s.append("MUL"); break;
            case DIV: s.append("DIV"); break;
            case MOD: s.append("MOD"); break;

            case STORE: s.append("STORE"); break;
            case RETRIEVE: s.append("RETRIEVE"); break;

            case MARK: s.append("MARK"); break;
            case CALL: s.append("CALL"); break;
            case JUMP: s.append("JUMP"); break;
            case JUMPZERO: s.append("JUMPZERO"); break;
            case JUMPNEG: s.append("JUMPNEG"); break;
            case ENDSUB: s.append("ENDSUB"); break;
            case ENDPROG: s.append("ENDPROG"); break;

            case WRITEC: s.append("WRITEC"); break;
            case WRITEN: s.append("WRITEN"); break;
            case READC: s.append("READC"); break;
            case READN: s.append("READN"); break;
            default: throw instructionNotFoundException;
        }
        switch(p[k]) {
		    case PUSH: case COPY: case SLIDE: case MARK:
		    case CALL: case JUMP: case JUMPZERO: case JUMPNEG:
		        s.append(to_string(p[++k])); break;
		    default: break;
        }
        s.append("\n");
    }
    return s;
}

int main() {
    string fileContents = readFile("hello_world.ws");
    auto tokens = tokenise(fileContents);
    printTokens(tokens);
    cout << endl;
    auto program = tokensToProgram(tokens);
    string textrep = programToString(program);
    cout << textrep << endl;
    return 0;
}
