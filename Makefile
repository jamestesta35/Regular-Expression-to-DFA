all: ls_main regex_main

dfa.o: dfa.cpp
	g++ -c dfa.cpp

expression.o: expression.cpp
	g++ -c expression.cpp

finiteMachine.o: finiteMachine.cpp
	g++ -c finiteMachine.cpp

ls_main.o: ls_main.cpp
	g++ -c ls_main.cpp

ls_main: ls_main.o finiteMachine.o expression.o
	g++ -o ls_main ls_main.o finiteMachine.o expression.o

regex_main.o: regex_main.cpp
	g++ -c regex_main.cpp

regex_main: regex_main.o finiteMachine.o expression.o
	g++ -o regex_main regex_main.o finiteMachine.o expression.o

clean:
	rm *.o regex_main ls_main
