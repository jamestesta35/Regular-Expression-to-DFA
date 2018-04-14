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
  bool run_sub(string currentState, string input);
  bool run_eps(string currentState, string input, vector<string> epsilons);
  bool isAcceptState(string state);
  vector<string> findTransitions(string currentState, char input);
  vector<string> findEmptyStateList(string state, vector<string> seenStates);
  finiteMachine();
  finiteMachine(vector<string> states,
		vector<char> alphabet,
		map<string,map<char,vector<string> > >transitionTable,
		string startState,
		vector<string> acceptStates);
  finiteMachine(string regex);
  finiteMachine(expression ex);
  bool run(string input);
  finiteMachine toDFA();
  string getStartState();
  bool hasAcceptState(vector<string> strings);
  string findStateName(vector<string> strings);
  void printMachine();
  bool printJson(string file);
  bool loadJson(string file);
   
};

#endif // FINITEMACHINE_H
