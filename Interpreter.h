#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <list>
#include <map>
#include <vector>
#include <iostream>
#include "Types.h"
#include "Exceptions.h"

class Interpreter {
    public:
        Interpreter(Program);
        void interpret();

    private:
        Program p; // Contains instructions from the Whitespace source
        std::vector<long long> heap;
        std::list<long long> stack; // To store values
        std::vector<size_t> callStack; // To remember where to return to
        std::map<long long, size_t> labels; // Lookup table for labels
};

#endif
