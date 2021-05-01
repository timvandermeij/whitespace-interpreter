#ifndef TYPES_H
#define TYPES_H

enum InstructionType {
    PUSH, DUP, COPY, SWAP, DISCARD, SLIDE, // Stack manipulations
    ADD, SUB, MUL, DIV, MOD, // Arithmetic operations
    STORE, RETRIEVE, // Heap access
    MARK, CALL, JUMP, JUMPZERO, JUMPNEG, ENDSUB, ENDPROG, // Flow control
    WRITEC, WRITEN, READC, READN // I/O operations
};

enum Token {
    LINEFEED, SPACE, TAB
};

enum Mode {
    STACKMANIP, ARITH, HEAPACC, FLOWCONT, IO
};

struct Instruction {
    InstructionType type;
    long long arg;

    Instruction(InstructionType type, long long arg = 0) : type(type), arg(arg) {}
};

typedef std::vector<Instruction> Program;

#endif
