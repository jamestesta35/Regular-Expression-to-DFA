#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include "finiteMachine.h"

using namespace std;

finiteMachine::finiteMachine(){
  isDFA = false;
}

finiteMachine::finiteMachine(vector<string> states, vector<char> alphabet, map<string,map<char,vector<string> > > transitionTable, string startState, vector<string> acceptStates)
{
  states_ = states;
  alphabet_ = alphabet;
  transitionTable_ = transitionTable;
  startState_ = startState;
  acceptStates_ = acceptStates;
  isDFA = false;
}

finiteMachine::finiteMachine(string regex){
  expression ex(regex);
  string start = "q0";
  states_.push_back(start);
  startState_ = start;
  string finalState = ex.toNFA(start, &states_, &transitionTable_, &alphabet_);
  acceptStates_.push_back(finalState);
  isDFA = false;
}

finiteMachine::finiteMachine(expression ex){
  string start = "q0";
  states_.push_back(start);
  startState_ = start;
  string finalState = ex.toNFA(start, &states_, &transitionTable_, &alphabet_);
  acceptStates_.push_back(finalState);
  isDFA = false;
}

bool run2(string input, map<string,map<char,vector<string> > > transitionTable, string startState, vector<string> acceptStates, vector<char>alphabet){
  string state = startState;
  vector<string> states;
  for(unsigned i = 0; i < input.size(); ++i){
    if(find(alphabet.begin(), alphabet.end(), input[i]) == alphabet.end()){
      return false;
    }
    states = transitionTable[state][input[i]];
    if(states.size() == 0){
      return false;
    }
    state = states[0];
  }
  if(find(acceptStates.begin(), acceptStates.end(), state) == acceptStates.end()){
    return false;
  }
  return true;
}

bool finiteMachine::run(string input){
  if(!isDFA){
    vector<string> epsilons;
    string startState = getStartState();
    return run_eps(startState, input, epsilons);
  }
  return run2(input, transitionTable_, startState_, acceptStates_, alphabet_);
}



finiteMachine finiteMachine::toDFA(){ 
  // We already have the transition table
  map<string,map<char,vector<string> > > newTransitionTable;
  vector<string> newStates;
  vector< vector<string> > newStates_vec;
  vector<string> newAcceptStates;
  string newStartState;
  
  vector<string> currentStates;

  vector<string> temp;
  currentStates = findEmptyStateList(startState_, temp);
  newStates_vec.push_back(currentStates);
  
  newStartState = findStateName(currentStates);
  //cout << "Adding new State: " << newStartState << "\n";
  for(unsigned i = 0; i < acceptStates_.size(); ++i){
    if(find(currentStates.begin(), currentStates.end(), acceptStates_[i]) != currentStates.end()){
      newAcceptStates.push_back(newStartState);
      i = acceptStates_.size();
    }
  }
  
  for(unsigned i = 0; i < newStates_vec.size(); ++i){
    //Check to see if it already exists as a state
    currentStates = newStates_vec[i];
    string currentName = findStateName(currentStates);
    if (find(newStates.begin(), newStates.end(), currentName) == newStates.end()){
      //We have a new state and need to find all the possible routs
      newStates.push_back(currentName);
      //Start with the first letter 
      for(unsigned j = 0; j < alphabet_.size(); ++j){
	char currentAlphabet = alphabet_[j];
	vector<string> outcomes;
	//Then take each of the states it could be in
	for(unsigned k = 0; k < currentStates.size(); ++k){
	  //Then add to a list all of the possible states you could end up next
	  vector<string> newOutcomes = findTransitions(currentStates[k], currentAlphabet);
	  //Use the empty transition state for those to find the new states
	  for(unsigned l = 0; l < newOutcomes.size(); ++l){
	    vector<string> temp_;
	    vector<string> newOutcomesEmpty = findEmptyStateList(newOutcomes[l], temp_);
	    for(unsigned m = 0; m < newOutcomesEmpty.size(); ++m){
	      if(find(outcomes.begin(), outcomes.end(), newOutcomesEmpty[m]) == outcomes.end()){
		outcomes.push_back(newOutcomesEmpty[m]);
	      }
	    }
	  }
	}  // end of loop of current States
	//Now that we have all of the outcomes for the alphabet,
	newStates_vec.push_back(outcomes);
	string newState = findStateName(outcomes);
	if(outcomes.size() != 0){
	  //We have the new state so add it to the transition table
	  newTransitionTable[currentName][currentAlphabet].push_back(newState);
	  //See if the new state will be an accept state
	  if(hasAcceptState(outcomes)){
	    if(find(newAcceptStates.begin(), newAcceptStates.end(), newState) == newAcceptStates.end())
	      newAcceptStates.push_back(newState);
	  }
	}
      } // End of alphabet loop

    }// end of if statement
    
  }// end of states_vec loop

  //TO DO: Maybe take out the empty state name. It isnt a big deal but could look better
  
  finiteMachine newMachine = finiteMachine(newStates, alphabet_, newTransitionTable, newStartState, newAcceptStates);
  newMachine.isDFA = true;
  
  return newMachine;
}

bool finiteMachine::run_sub(string currentState, string input){
  //std::cout << "In sub in " << currentState << " State:" << input << "\n";
  if (input.empty()){
    return isAcceptState(currentState);
  } else {
    vector<string> transitions = findTransitions(currentState, input[0]);
    if (transitions.empty()){
      return false;
    } else {
      bool results = false;
      string newString = input.erase(0,1);
      for (unsigned i = 0; i < transitions.size(); ++i){
	vector<string> epsilons;
	string newState = transitions[i];
	results = results | run_eps(newState, newString, epsilons);
      }
      return results;
    }
  }
}

bool finiteMachine::run_eps(string currentState, string input, vector<string> epsilons){
  //std::cout << "In eps in " << currentState << " State:" << input << "\n";
  for (unsigned i = 0; i < epsilons.size(); ++i){
    if (epsilons[i] == currentState){
      // We must have already been at this state so we must have a loop
      return false;
    }
  }
  //std::cout << "I have not already been here\n";
  epsilons.push_back(currentState);
  bool results = false;
  vector<string> transitions = findTransitions(currentState, ' ');
  for (unsigned i = 0; i < transitions.size(); ++i){
    //This must be an epsilon state so we want to add it to the list and then 
    // check that one for susiquent epsilons
    string nexState = transitions[i];
    //std::cout << "Going to node " << transitionTable_[transitions[i]].next << "\n";
    results = results | run_eps(nexState, input, epsilons);
  }
  // Now that we have already gotten all of the subsequent epsilons,
  // we need to take the run of this state.
  return results | run_sub(currentState, input);
}

bool finiteMachine::isAcceptState(string state){
  for (unsigned i = 0; i < acceptStates_.size(); ++i){
    if (acceptStates_[i] == state){
      return true;
    }
  }
  return false;
}

vector<string> finiteMachine::findTransitions(string currentState, char input){
  return transitionTable_[currentState][input];
}

string finiteMachine::getStartState(){
  return startState_;
}

vector<string> finiteMachine::findEmptyStateList(string state, vector<string> seenStates){
  if(find(seenStates.begin(), seenStates.end(), state) != seenStates.end()){
    return seenStates;
  }
  vector<string> emptyTransitions = transitionTable_[state][' '];
  seenStates.push_back(state);
  for(unsigned i = 0; i < emptyTransitions.size(); ++i){
    vector<string> returnedStates = findEmptyStateList(emptyTransitions[i], seenStates);
    for(unsigned j = 0; j < returnedStates.size(); ++j){
      if(find(seenStates.begin(), seenStates.end(), returnedStates[j]) == seenStates.end()){
	seenStates.push_back(returnedStates[j]);
      }
    }
  }
  return seenStates;
}

string finiteMachine::findStateName(vector<string> strings){
  string name = "";
  sort(strings.begin(), strings.end());
  for(unsigned l = 0; l < strings.size(); ++l){
    name += strings[l];
    if(l != strings.size()-1){
      name += ",";
    }
  }
  return name;
}

bool finiteMachine::hasAcceptState(vector<string> strings){
  bool accept = false;
  for(unsigned l = 0; l < strings.size(); ++l){
    if(find(acceptStates_.begin(), acceptStates_.end(), strings[l]) != acceptStates_.end()){
      accept = true;
    }
  }
  return accept;
}

void finiteMachine::printMachine(){
  for(unsigned i = 0; i < states_.size(); ++i){
    map<char, vector <string > > tempMap = transitionTable_[states_[i]];
    for(int j = 0; j < 127; ++j){
      vector<string> nextState = tempMap[char(j)];
      if(!nextState.empty()){
	for(unsigned k = 0 ; k < nextState.size(); ++k){
	  cout << "Transition-> " << states_[i] << " " << char(j) << " " << nextState[k] << endl;
	}
      }
    }
  }
  for(unsigned i = 0; i < states_.size(); ++i){
    cout << "State-> "<< states_[i] << endl;
  }
  for(unsigned i = 0; i < acceptStates_.size(); ++i){
    cout << "End States-> "<< acceptStates_[i] << endl;
  }
  cout << "StartState = " << startState_ << "\n";
  for(unsigned i = 0; i < alphabet_.size(); ++i){
    cout << "Alphabet -> "<< alphabet_[i] << endl;
  }
}

void finiteMachine::printJson(string file){
  cout << "{\n\t";
  if(isDFA){
    cout << "\"dfa\": {\n\t\t";
  } else {
    cout << "\"nfa\": {\n\t\t";
  }
  cout << "\"states\": {\n\t\t\t";
  cout << "\"state\": [\n";
  for(unsigned i = 0; i < states_.size(); ++i){
    cout << "\t\t\t\t{\n\t\t\t\t\t\"name\": \"";
    cout << states_[i];
    cout << "\"\n\t\t\t\t}";
    if(i != states_.size()){
      cout << ",";
    }
    cout << "\n";
  }
  cout << "\t\t\t]\n\t\t},\n\t\t\"alphabet\": {\n";
  cout << "\t\t\t\"input\": [\n";
  for(unsigned i = 0; i < alphabet_.size(); ++i){
    cout << "\t\t\t\t{\n\t\t\t\t\t\"char\": \"";
    cout << alphabet_[i];
    cout << "\"\n\t\t\t\t}";
    if(i != alphabet_.size()){
      cout << ",";
    }
    cout << "\n";
  }
}
