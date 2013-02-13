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
        long tokensToNumber(const std::vector<Token> &, int &);
        void parseNumber(const std::vector<Token> &, Program &, int &);
        void processStackManip(const std::vector<Token> &, Program &, int &);
        void processArith(const std::vector<Token> &, Program &, int &);
        void processHeapAcc(const std::vector<Token> &, Program &, int &);
        void processFlowCont(const std::vector<Token> &, Program &, int &);
        void processIO(const std::vector<Token> &, Program &, int &);
};

#endif
