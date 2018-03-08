all: main expression newls

main: main.o dfa.o
	g++ -o main main.o dfa.o

main.o: main.cpp
	g++ -c main.cpp

dfa.o: dfa.cpp
	g++ -c dfa.cpp

expression: expression.o mainExpression.o finiteMachine.o
	g++ -o expression expression.o mainExpression.o finiteMachine.o

expression.o: expression.cpp
	g++ -c expression.cpp

mainExpression.o: mainExpression.cpp
	g++ -c mainExpression.cpp

finiteMachine.o: finiteMachine.cpp
	g++ -c finiteMachine.cpp

newls: newls.o
	g++ -o newls newls.o

newls.o: newls.cpp
	g++ -c newls.cpp

clean:
	rm *.o
