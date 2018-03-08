#include <algorithm>
#include <map>
#include <string>
#include <vector>
#include "dfa.h"

using namespace std;

dfa::dfa(vector<string> states, vector<char> alphabet, map<string, map<char, string> > transitionTable, string startState, vector<string> endStates){
  states_ = states;
  alphabet_ = alphabet;
  transitionTable_ = transitionTable;
  startState_ = startState;
  endStates_ = endStates;
}

bool dfa::checkInput(string input){
  string currentState = startState_;
  for(int i = 0; i < input.size(); ++i){
    if(find(alphabet_.begin(), alphabet_.end(), input[i]) == alphabet_.end()){
      return false;
    }
    string newState = transitionTable_[currentState][input[i]];
    if(newState == ""){
      return false;
    }
    currentState = newState;
  }
  if(find(endStates_.begin(), endStates_.end(), currentState) != endStates_.end()){
    return true;
  }
  return false;
}

