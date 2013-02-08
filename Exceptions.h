#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>

class UnreachableTokenException: public std::exception {
    virtual const char *what() const throw () {
        return "Error: unreachable token.";
    }
} unreachableTokenException;

class NoArgumentException: public std::exception {
};

class NoNumericArgumentException: public NoArgumentException {
    virtual const char *what() const throw () {
        return "Error: no numeric argument was passed.";
    }
} noNumericArgumentException;

class NoLabelArgumentException: public NoArgumentException {
    virtual const char *what() const throw () {
        return "Error: no label argument was passed.";
    }
} noLabelArgumentException;

class OutOfBoundsException: public NoArgumentException {
    virtual const char *what() const throw () {
        return "Error: index out of bounds.";
    }
} outOfBoundsException;

class PrematureEndException: public std::exception {
    virtual const char *what() const throw () {
        return "Error: number or label ended prematurely.";
    }
} prematureEndException;

class UndefinedSignException: public std::exception {
    virtual const char *what() const throw () {
        return "Error: sign of the number is undefined.";
    }
} undefinedSignException;

class InstructionNotFoundException: public std::exception {
    virtual const char *what() const throw () {
        return "Error: instruction has not been found.";
    }
} instructionNotFoundException;

class LabelNotFoundException: public std::exception {
    virtual const char *what() const throw () {
        return "Error: label has not been found.";
    }
} labelNotFoundException;

#endif
