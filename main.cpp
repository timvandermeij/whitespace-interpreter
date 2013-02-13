#include <cstdlib>
#include <fstream>

#include "Parser.h"
#include "Exceptions.h"

using namespace std;

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
    string fileContents = readFile("hello_world.ws");
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
