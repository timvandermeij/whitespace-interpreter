#include <cstdlib>
#include <cctype>
#include <fstream>

#include "Parser.h"
#include "Exceptions.h"

using namespace std;

void printTokens(const vector<Token> &tokens) {
    size_t length = tokens.size();

    for(size_t k = 0; k < length; k++) {
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

Program stringToProgram(string program) {
    size_t size = program.size();
    Program p;
    for(size_t k = 0; k < size; k++) {
        while(program[k] == ' ' || program[k] == '\n') k++; // ignore whitespace
        if(k >= size) throw SomeException(); // program consisted only of spaces and newlines
        if(program[k] == 'P') {
            if(program.find("PUSH", k) == k) {
                k += 4; // advance to the character behind PUSH
                while(program[k++] == ' ' && k < size); // ignore any spaces
                if(k >= size || program[k] == '\n') throw SomeException();
                string d;
                while(isdigit(program[k])) {
                    d += program[k];
                    k++;
                    if(k >= size) throw SomeException();
                }
                long number = atoi(d.c_str());
                p.push_back(PUSH);
                p.push_back((Instruction)number);
            } else throw SomeException();
        } else if(program[k] == 'A') {
            if(program.find("ADD", k) == k) {
              p.push_back(ADD);
            } else throw SomeException();
        } else if(program[k] == 'C') {
            if(k + 4 < size) {
                k += 4;
                if(program.find("COPY", k) == k) {
                    p.push_back(COPY);
                } else if(program.find("CALL", k) == k) {
                    p.push_back(CALL);
                    while(program[k++] == ' ' && k < size); // ignore any spaces
                    if(k >= size || program[k] == '\n') throw SomeException();
                    string d;
                    while(isdigit(program[k])) {
                        d += program[k];
                        k++;
                        if(k >= size) throw SomeException();
                    }
                    long number = atoi(d.c_str());
                    p.push_back((Instruction)number);
                } else throw SomeException();
            } else throw SomeException();
        } else if(program[k] == 'D') {
            if(program.find("DISCARD", k) == k) {
                p.push_back(DISCARD);
                k += 7;
            } if(program.find("DIV", k) == k) {
                p.push_back(DIV);
                k += 3;
            } else if(program.find("DUP", k) == k) {
                p.push_back(DUP);
                k += 3;
            } else throw SomeException();
        } else if(program[k] == 'E') {
            if(program.find("ENDPROG", k) == k) {
                p.push_back(ENDPROG);
                k += 7;
            } else if(program.find("ENDSUB", k) == k) {
                p.push_back(ENDSUB);
                k += 6;
            } throw SomeException();
        } else throw SomeException();
        while(k < size && program[k++] == ' ');
        if(k >= size || program[++k] != '\n') throw SomeException(); // there should be a newline here
    }
    return p;
}

string programToString(Program p) {
    size_t size = p.size();
    string s;

    for(size_t k = 0; k < size; k++) {
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
            default: throw InstructionNotFoundException();
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

int main() {
    // Load the Whitespace source file and tokenize it.
    Parser parser;
    string fileContents = readFile("hello_worldvanwiki.ws");
    auto tokens = parser.tokenize(fileContents);
    printTokens(tokens);
    cout << endl;

    // Print the tokens in an assembly-like way.
    auto program = parser.tokensToProgram(tokens);
    string textrep = programToString(program);
    cout << textrep << endl;

    // Interpret the Whitespace source file.
    Interpreter interpreter(program);
    interpreter.interpret();

    return 0;
}
