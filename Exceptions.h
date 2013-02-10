#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>

class UnreachableTokenException: public std::exception {
    virtual const char *what() const throw () {
        return "Error: unreachable token.";
    }
};
extern const UnreachableTokenException unreachableTokenException;

class NoArgumentException: public std::exception {
};

class NoNumericArgumentException: public NoArgumentException {
    virtual const char *what() const throw () {
        return "Error: no numeric argument was passed.";
    }
};

extern const NoNumericArgumentException noNumericArgumentException;

class NoLabelArgumentException: public NoArgumentException {
    virtual const char *what() const throw () {
        return "Error: no label argument was passed.";
    }
};

extern const NoLabelArgumentException noLabelArgumentException;

class OutOfBoundsException: public NoArgumentException {
    virtual const char *what() const throw () {
        return "Error: index out of bounds.";
    }
};

extern const OutOfBoundsException outOfBoundsException;

class PrematureEndException: public std::exception {
    virtual const char *what() const throw () {
        return "Error: number or label ended prematurely.";
    }
};

extern const PrematureEndException prematureEndException;

class UndefinedSignException: public std::exception {
    virtual const char *what() const throw () {
        return "Error: sign of the number is undefined.";
    }
};

extern const UndefinedSignException undefinedSignException;

class InstructionNotFoundException: public std::exception {
    virtual const char *what() const throw () {
        return "Error: instruction has not been found.";
    }
};

extern const InstructionNotFoundException instructionNotFoundException;

class LabelNotFoundException: public std::exception {
    virtual const char *what() const throw () {
        return "Error: label has not been found.";
    }
};

extern const LabelNotFoundException labelNotFoundException;

#endif
