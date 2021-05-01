#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <string>
#include <iostream>
#include <cmath>

#include "Interpreter.h"
#include "Exceptions.h"

class Parser {
    public:
        std::vector<Token> tokenize(const std::string &);
        Program tokensToProgram(const std::vector<Token> &);

    private:
        Mode determineMode(const Token, const Token);
        long long tokensToNumber(const std::vector<Token> &, size_t &);
        void processStackManip(const std::vector<Token> &, Program &, size_t &);
        void processArith(const std::vector<Token> &, Program &, size_t &);
        void processHeapAcc(const std::vector<Token> &, Program &, size_t &);
        void processFlowCont(const std::vector<Token> &, Program &, size_t &);
        void processIO(const std::vector<Token> &, Program &, size_t &);
};

#endif
