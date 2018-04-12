all: ls_main regex_main

flags = -std=c++11 -Wall -Werror

expression.o: expression.cpp
	g++ $(flags) -c expression.cpp

finiteMachine.o: finiteMachine.cpp
	g++ $(flags) -c finiteMachine.cpp

ls_main.o: ls_main.cpp
	g++ $(flags) -c ls_main.cpp

ls_main: ls_main.o finiteMachine.o expression.o
	g++ $(flags) -o ls_main ls_main.o finiteMachine.o expression.o

regex_main.o: regex_main.cpp
	g++ $(flags) -c regex_main.cpp

regex_main: regex_main.o finiteMachine.o expression.o
	g++ $(flags) -o regex_main regex_main.o finiteMachine.o expression.o

clean:
	rm *.o regex_main ls_main
