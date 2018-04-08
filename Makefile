all: ls_main regex_main

dfa.o: dfa.cpp
	g++ -std=c++11 -Wall -Werror -c dfa.cpp 

expression.o: expression.cpp
	g++ -std=c++11 -Wall -Werror -c expression.cpp

finiteMachine.o: finiteMachine.cpp
	g++ -std=c++11 -Wall -Werror -c finiteMachine.cpp

ls_main.o: ls_main.cpp
	g++ -std=c++11 -Wall -Werror -c ls_main.cpp

ls_main: ls_main.o finiteMachine.o expression.o
	g++ -std=c++11 -Wall -Werror -o ls_main ls_main.o finiteMachine.o expression.o

regex_main.o: regex_main.cpp
	g++ -std=c++11 -Wall -Werror -c regex_main.cpp

regex_main: regex_main.o finiteMachine.o expression.o
	g++ -std=c++11 -Wall -Werror -o regex_main regex_main.o finiteMachine.o expression.o

clean:
	rm *.o regex_main ls_main
