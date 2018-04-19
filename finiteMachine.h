#ifndef FINITEMACHINE_H
#define FINITEMACHINE_H

#include <map>
#include <string>
#include <vector>
#include "expression.h"

using namespace std;



class finiteMachine{
 public:
  vector<string> states_;
  vector<char> alphabet_;
  map<string,map<char,vector<string> > > transitionTable_;
  string startState_;
  vector<string> acceptStates_;
  bool isDFA;
  
/*
  Function: run_sub()
  Arguments: string currentState - state were looking at
             string input - input that we current still have to look at
  Returns: bool - if the rest of the input is accepting
  Description: checks if there is a transition to another state from the current state
*/
  bool run_sub(string currentState, string input);
  
/*
  Function: run_eps()
  Arguments: string currentState - state were looking at
             string input - input that we current still have to look at
  Returns: bool - if the rest of the input is accepting
  Description: checks if there is an empty transition to another state from the current state
*/
  bool run_eps(string currentState, string input, vector<string> epsilons);
  
/*
  Function: isAcceptState()
  Arguments: string state - state were looking at
  Returns: bool - if the state is an accepting state
  Description: checks if a state is in the set of accept states
*/
  bool isAcceptState(string state);
  
/*
  Function: findTranstitions()
  Arguments: string currentState - state were looking at
             char input - input character
  Returns: vector<string> - all the transitions
  Description: returns all the transitions for the current state given the input
*/
  vector<string> findTransitions(string currentState, char input);
  
/*
  Function: findEmptyStateList()
  Arguments: string currentState - state were looking at
             char input - input character
  Returns: vector<string> - all the transitions
  Description: returns all the empty transitions for the current state given the input
*/
  vector<string> findEmptyStateList(string state, vector<string> seenStates);

/*
  Function: finiteMachine()
  Arguments: none
  Returns: none
  Description: Basic constructor for finiteMachine
*/
  finiteMachine();

/*
  Function: finiteMachine()
  Arguments: vector<string> states
	     vector<char> alphabet
	     map<string,map<char,vector<string> > >transitionTable
	     string startState
	     vector<string> acceptStates
  Returns: none
  Description: Constructor for finiteMachine with the 5 tuple arguments
*/
  finiteMachine(vector<string> states,
		vector<char> alphabet,
		map<string,map<char,vector<string> > >transitionTable,
		string startState,
		vector<string> acceptStates);

/*
  Function: finiteMachine()
  Arguments: string regex - the regular expression to convert
  Returns: none
  Description: Constructor for finiteMachine to make a regular expression
*/
  finiteMachine(string regex);

/*
  Function: finiteMachine()
  Arguments: expression ex - the regular expression object to convert
  Returns: none
  Description: Constructor for finiteMachine to make a regular expression from an expression object
*/
  finiteMachine(expression ex);

/*
  Function: run()
  Arguments: string input - input for the machine
  Returns: bool - if the input is accepted
  Description: Checks if the input is part of the language of the machine
*/
  bool run(string input);

/*
  Function: toDFA()
  Arguments: none
  Returns: finiteMachine - converted DFA machine
  Description: Converts the current machine into a DFA
*/
  finiteMachine toDFA();

/*
  Function: getStartState()
  Arguments: none
  Returns: string - start state name
  Description: Returns the start state of the machine
*/ 
  string getStartState();

/*
  Function: hasAcceptState()
  Arguments: vector<string> strings - list of states
  Returns: bool - if the states hold an acceptState
  Description: See if a set of states holds a final state
*/
  bool hasAcceptState(vector<string> strings);

/*
  Function: findStateName()
  Arguments: vector<string> strings - list of states
  Returns: string - state name with all the sub states
  Description: Combines the states and gives a new name
*/
  string findStateName(vector<string> strings);

/*
  Function: printMachine()
  Arguments: none
  Returns: none
  Description: Prints contents of the machine to the screen
*/
  void printMachine();

/*
  Function: printJson()
  Arguments: string file - file to print to
  Returns: bool - if the file was printed to correctly
  Description: Print the machine to a file in Json format
*/
  bool printJson(string file);

/*
  Function: loadJson()
  Arguments: string file - file to load from
  Returns: bool - if the file was loaded correctly
  Description: Load the machine from a file in Json format
*/
  bool loadJson(string file);
   
};

#endif // FINITEMACHINE_H
