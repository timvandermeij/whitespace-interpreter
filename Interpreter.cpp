#include "Interpreter.h"

using namespace std;

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
                int first, second;
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
                int top;
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
            case CALL: { // Not sure what the difference is between CALL and JUMP. I think this CALL function should actually execute a function...
                int label = p[++pc];
                auto pair = labels.find(label);
                if(pair == labels.end()) { // Is this correct? Probably fetches last item, which is not what we want...
                    throw labelNotFoundException;
                }
                callStack.push_back(pc);
                pc = pair->second;
                break;
            }
            case JUMP: {
                int label = p[++pc];
                auto pair = labels.find(label);
                if(pair == labels.end()) { // Is this correct? Probably fetches last item, which is not what we want...
                    throw labelNotFoundException;
                }
                callStack.push_back(pc);
                pc = pair->second;
                break;
            }
            case JUMPZERO: {
                if(stack.front() == 0) {
                    int label = p[++pc];
                    auto pair = labels.find(label);
                    if(pair == labels.end()) { // Is this correct? Probably fetches last item, which is not what we want...
                        throw labelNotFoundException;
                    }
                    callStack.push_back(pc);
                    pc = pair->second;
                }
                break;
            }
            case JUMPNEG: {
                if(stack.front() < 0) {
                    int label = p[++pc];
                    auto pair = labels.find(label);
                    if(pair == labels.end()) { // Is this correct? Probably fetches last item, which is not what we want...
                        throw labelNotFoundException;
                    }
                    callStack.push_back(pc);
                    pc = pair->second;
                }
                break;
            }
            case ENDSUB: {
                pc = callStack.back();
                callStack.pop_back();
                break;
            }
            case ENDPROG: {
                cout << endl << endl << "Press the Enter key to exit..." << endl;
                cin.get();
                cin.ignore();
                exit(0);
            }

            // I/O operations
            case WRITEC: {
                cout << (char)stack.front() << endl;
                stack.pop_front();
                break;
            }
            case WRITEN: {
                cout << (int)stack.front() << endl;
                stack.pop_front();
                break;
            }
            case READC: {
                char character;
                cin >> character;
                heap.push_back(character);
                break;
            }
            case READN: {
                int number;
                cin >> number;
                heap.push_back(number);
                break;
            }
            default:
                throw instructionNotFoundException;
        }
    }
}
