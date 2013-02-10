#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>

class unreachableTokenException: public std::exception {
    virtual const char *what() const throw () {
        return "Error: unreachable token.";
    }
};

class noArgumentException: public std::exception {
};

class noNumericArgumentException: public noArgumentException {
    virtual const char *what() const throw () {
        return "Error: no numeric argument was passed.";
    }
};

class noLabelArgumentException: public noArgumentException {
    virtual const char *what() const throw () {
        return "Error: no label argument was passed.";
    }
};

class outOfBoundsException: public noArgumentException {
    virtual const char *what() const throw () {
        return "Error: index out of bounds.";
    }
};

class prematureEndException: public std::exception {
    virtual const char *what() const throw () {
        return "Error: number or label ended prematurely.";
    }
};

class undefinedSignException: public std::exception {
    virtual const char *what() const throw () {
        return "Error: sign of the number is undefined.";
    }
};

class instructionNotFoundException: public std::exception {
    virtual const char *what() const throw () {
        return "Error: instruction has not been found.";
    }
};

class labelNotFoundException: public std::exception {
    virtual const char *what() const throw () {
        return "Error: label has not been found.";
    }
};

#endif
