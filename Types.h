#ifndef TYPES_H
#define TYPES_H

enum Instruction {
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

typedef std::vector<Instruction> Program;

#endif
