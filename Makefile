FLAGS = -std=c++11 -fpermissive

all: main.o Parser.o Interpreter.o
	g++ -Wall -Wextra -ggdb $(FLAGS) -o whitespace main.o Parser.o Interpreter.o
Parser.o: Parser.cpp Parser.h Exceptions.h Types.h
	g++ -Wall -Wextra -ggdb $(FLAGS) -c Parser.cpp
Interpreter.o: Interpreter.cpp Interpreter.h Types.h
	g++ -Wall -Wextra -ggdb $(FLAGS) -c Interpreter.cpp
main.o: main.cpp Parser.h Interpreter.h Exceptions.h
	g++ -Wall -Wextra -ggdb $(FLAGS) -c main.cpp
clean:
	rm *.o whitespace