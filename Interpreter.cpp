#include "Interpreter.h"
#include "Exceptions.h"

using namespace std;

Interpreter::Interpreter(Program p) {
    this->p = p;
}

void Interpreter::interpret() {
    size_t pc, size = p.size();

    for(pc = 0; pc < size; pc++) {
        if(p[pc].type == MARK) {
            labels.insert(pair<int, unsigned>(p[pc].arg, pc + 1)); // Go to next instruction after label
        }
    }

    for(pc = 0; pc < size; ) {
        Instruction inst = p[pc++];
        switch(inst.type) {
            // Stack manipulations
            case PUSH: {
                stack.push_front(inst.arg);
                break;
            }
            case DUP: {
                stack.push_front(stack.front());
                break;
            }
            case COPY: {
                auto it = stack.begin();
                advance(it, inst.arg);
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
                int top = stack.front();
                for(int i = 0; i <= inst.arg; i++) {
                    stack.pop_front();
                }
                stack.push_front(top);
                break;
            }

            // Arithmetic
            case ADD: {
                int a = stack.front();
                stack.pop_front();
                stack.front() += a;
                break;
            }
            case SUB: {
                int a = stack.front();
                stack.pop_front();
                stack.front() -= a;
                break;
            }
            case MUL: {
                int a = stack.front();
                stack.pop_front();
                stack.front() *= a;
                break;
            }
            case DIV: {
                int a = stack.front();
                stack.pop_front();
                stack.front() /= a;
                break;
            }
            case MOD: {
                int a = stack.front();
                stack.pop_front();
                stack.front() %= a;
                break;
            }

            // Heap access
            case STORE: {
                int value = stack.front();
                stack.pop_front();
                size_t address = stack.front();
                stack.pop_front();
                if(address < 0) {
                    throw OutOfBoundsException();
                }
                if(heap.size() <= address) {
                    heap.resize(address + 1, 0);
                }
                heap[address] = value;
                break;
            }
            case RETRIEVE: {
                int address = stack.front();
                stack.pop_front();
                int size = heap.size();
                if(address < 0) {
                    throw OutOfBoundsException();
                } else if(size <= address) {
                    stack.push_front(0);
                } else {
                    stack.push_front(heap[address]);
                }
                break;
            }

            // Flow control
            case MARK: {
                break;
            }
            case CALL: {
                auto pair = labels.find(inst.arg);
                if(pair == labels.end()) {
                    throw LabelNotFoundException();
                }
                callStack.push_back(pc);
                pc = pair->second; // go to the instruction at the found program counter
                break;
            }
            case JUMP: {
                auto pair = labels.find(inst.arg);
                if(pair == labels.end()) {
                    throw LabelNotFoundException();
                }
                pc = pair->second;
                break;
            }
            case JUMPZERO: {
                if(stack.front() == 0) {
                    auto pair = labels.find(inst.arg);
                    if(pair == labels.end()) {
                        throw LabelNotFoundException();
                    }
                    pc = pair->second;
                }
                stack.pop_front();
                break;
            }
            case JUMPNEG: {
                if(stack.front() < 0) {
                    auto pair = labels.find(inst.arg);
                    if(pair == labels.end()) {
                        throw LabelNotFoundException();
                    }
                    pc = pair->second;
                }
                stack.pop_front();
                break;
            }
            case ENDSUB: {
                pc = callStack.back();
                callStack.pop_back();
                break;
            }
            case ENDPROG: {
                exit(0);
                return; // this is officially the end of the interpreter session
            }

            // I/O operations
            case WRITEC: {
                cout << (char)stack.front();
                stack.pop_front();
                break;
            }
            case WRITEN: {
                cout << stack.front();
                stack.pop_front();
                break;
            }
            case READC: {
                char character = getchar();
                size_t address = stack.front();
                stack.pop_front();
                if (heap.size() <= address) {
                    heap.resize(address + 1, 0);
                }
                heap[address] = character;
                break;
            }
            case READN: {
                int number;
                cin >> number;
                size_t address = stack.front();
                stack.pop_front();
                if (heap.size() <= address) {
                    heap.resize(address + 1, 0);
                }
                heap[address] = number;
                break;
            }
            default:
                throw InstructionNotFoundException();
        }
    }
}
