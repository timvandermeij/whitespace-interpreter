WARN = -Wall -Wextra
DBG = -ggdb
FLAGS = -std=c++11 -fpermissive

all: main.o Parser.o Interpreter.o
	g++ $(WARN) $(DBG) $(FLAGS) -o whitespace main.o Parser.o Interpreter.o
Parser.o: Parser.cpp Parser.h Exceptions.h Types.h
	g++ $(WARN) $(DBG) $(FLAGS) -c Parser.cpp
Interpreter.o: Interpreter.cpp Interpreter.h Types.h
	g++ $(WARN) $(DBG) $(FLAGS) -c Interpreter.cpp
main.o: main.cpp Parser.h Interpreter.h Exceptions.h
	g++ $(WARN) $(DBG) $(FLAGS) -c main.cpp
clean:
	rm *.o whitespace
