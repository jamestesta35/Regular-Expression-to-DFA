#include <iostream>
#include <map>
#include <string>
#include <vector>
#include "expression.h"

using namespace std;

expression::expression(){
  t_star = false;
  t_or = false;
  t_term = false;
  character = "";
}

expression::expression(string regex){
  expression ex = parse_ex(regex);
  t_star = ex.t_star;
  t_or = ex.t_or;
  t_term = ex.t_term;
  character = ex.character;
  loe1 = ex.loe1;
  loe2 = ex.loe2;
}

string expression::toNFA(string startState, vector<string>* states, map<string, map<char, vector<string> > >* transitionTable, vector<char>* alphabet){
  if(t_term){
    //must be a term
    return termToNFA(character, startState, states, transitionTable, alphabet);
  } else if(t_or){
    //cout << "Checking type\n";
    return orToNFA(loe1, loe2, startState, states, transitionTable, alphabet);
  } else if(t_star) {
    return starToNFA(loe1, startState, states, transitionTable, alphabet);
  } else {
    string currentState = startState;
    for(int i = 0; i < loe1.size(); ++i){
      currentState = loe1[i].toNFA(currentState, states, transitionTable, alphabet);
    }
    return currentState;
  }
  return "EndTONFA";
}

string expression::starToNFA(vector<expression> loe1, string startState, vector<string>* states, map<string, map<char, vector <string> > >* transitionTable, vector<char>* alphabet){
  //Create a new state so signify we are doing the action (more than 0 times)
  string state1 = "q" + to_string(states->size());
  states->push_back(state1);
  transitionTable->operator[](startState)[' '].push_back(state1);
  //Loop though list so we can loop thougb mutliple times
  string currentState = state1;
  for(int i = 0; i < loe1.size(); ++i){
    currentState = loe1[i].toNFA(currentState, states, transitionTable, alphabet);
  }
  //Create a state for simplicity
  string state2 = "q" + to_string(states->size());
  states->push_back(state2);
  //Transition for simplicity
  transitionTable->operator[](currentState)[' '].push_back(state2);
  //Transition to skip this operation (0 times)
  transitionTable->operator[](startState)[' '].push_back(state2);
  //Transition to loop back
  transitionTable->operator[](state2)[' '].push_back(state1);
  
  return state2;
}

string expression::orToNFA(vector<expression> loe1, vector<expression> loe2, string startState, vector<string>* states, map<string, map<char, vector<string> > >* transitionTable, vector<char>* alphabet){
  //cout << "In OR" << endl;
  string currentState = startState;
  //Find the end state we will return
  string endState = "q" + to_string(states->size());
  states->push_back(endState);
  //create a link of states from loe1
  for(int i = 0; i < loe1.size(); ++i){
    currentState = loe1[i].toNFA(currentState, states, transitionTable, alphabet);
  }
  //link the list of states to the end state
  transitionTable->operator[](currentState)[' '].push_back(endState);

  //start back at the begining
  currentState = startState;
  //create the second link of states
  for(int i = 0; i < loe2.size(); ++i){
    currentState = loe2[i].toNFA(currentState, states, transitionTable, alphabet);
  }
  //connect the second link to the end state
  transitionTable->operator[](currentState)[' '].push_back(endState);
  
  return endState;
}

string expression::termToNFA(string character, string startState, vector<string>* states, map<string, map<char, vector<string> > >* transitionTable, vector<char>* alphabet){
  //cout << "In Term" << endl;
  string newState = "q" + to_string(states->size());
  states->push_back(newState);
  if(character.size() == 1){
    if(character[0] == '$' || character[0] == '^'){
      cerr << "Illegal character: " << character[0] << endl;
    } else if (character[0] == '\'' || character[0] == '['){
      cerr << "Character " << character[0] << "shouldnt be only character in term" << endl;
    } else if (character[0] == '.'){
      for(int i = ' '; i <= '~'; ++i){
	transitionTable->operator[](startState)[char(i)].push_back(newState);
	if(find(alphabet->begin(), alphabet->end(), char(i)) == alphabet->end()){
	  alphabet->push_back(char(i));
	}
      }
    } else {
      transitionTable->operator[](startState)[character[0]].push_back(newState);
      if(find(alphabet->begin(), alphabet->end(), character[0]) == alphabet->end()){
	alphabet->push_back(character[0]);
      }
    }
  } else {
    if(character[0] == '['){
      if(character[2] == '-'){
	int first = character[1];
	int second = character[3];
	for(int i = first; i <= second; ++i){
	  transitionTable->operator[](startState)[char(i)].push_back(newState);
	  if(find(alphabet->begin(), alphabet->end(), char(i)) == alphabet->end()){
	    alphabet->push_back(char(i));
	  }
	}
      } else {
	for(int i = 1; i < character.size()-1; ++i){
	  transitionTable->operator[](startState)[character[i]].push_back(newState);
	  if(find(alphabet->begin(), alphabet->end(), character[i]) == alphabet->end()){
	    alphabet->push_back(character[i]);
	  }
	}
      }
    } else if(character[0] == '\\'){
      switch (character[1]){
      case '.':
	transitionTable->operator[](startState)['.'].push_back(newState);
	if(find(alphabet->begin(), alphabet->end(), '.') == alphabet->end()){
	  alphabet->push_back('.');
	}
	break;
      case 'd':
	for(int j = '0'; j <= '9'; ++j){
	  transitionTable->operator[](startState)[char(j)].push_back(newState);
	  if(find(alphabet->begin(), alphabet->end(), char(j)) == alphabet->end()){
	    alphabet->push_back(char(j));
	  }
	}
	break;
      case 'w':
	for(int j = '0'; j <= '9'; ++j){
	  transitionTable->operator[](startState)[char(j)].push_back(newState);
	  if(find(alphabet->begin(), alphabet->end(), char(j)) == alphabet->end()){
	    alphabet->push_back(char(j));
	  }
	}
	for(int j = 'A'; j <= 'Z'; ++j){
	  transitionTable->operator[](startState)[char(j)].push_back(newState);
	  if(find(alphabet->begin(), alphabet->end(), char(j)) == alphabet->end()){
	    alphabet->push_back(char(j));
	  }
	}
	for(int j = 'a'; j <= 'z'; ++j){
	  transitionTable->operator[](startState)[char(j)].push_back(newState);
	  if(find(alphabet->begin(), alphabet->end(), char(j)) == alphabet->end()){
	    alphabet->push_back(char(j));
	  }
	}
	transitionTable->operator[](startState)['_'].push_back(newState);
	if(find(alphabet->begin(), alphabet->end(), '_') == alphabet->end()){
	  alphabet->push_back('_');
	}
	break;
      case 's':
	transitionTable->operator[](startState)[' '].push_back(newState);
	if(find(alphabet->begin(), alphabet->end(), ' ') == alphabet->end()){
	  alphabet->push_back(' ');
	}
	transitionTable->operator[](startState)[char(9)].push_back(newState);
	if(find(alphabet->begin(), alphabet->end(), char(9)) == alphabet->end()){
	  alphabet->push_back(char(9));
	}
	transitionTable->operator[](startState)[char(10)].push_back(newState);
	if(find(alphabet->begin(), alphabet->end(), char(10)) == alphabet->end()){
	  alphabet->push_back(char(10));
	}
	transitionTable->operator[](startState)[char(13)].push_back(newState);
	if(find(alphabet->begin(), alphabet->end(), char(13)) == alphabet->end()){
	  alphabet->push_back(char(13));
	}
	transitionTable->operator[](startState)[char(11)].push_back(newState);
	if(find(alphabet->begin(), alphabet->end(), char(11)) == alphabet->end()){
	  alphabet->push_back(char(11));
	}
	break;
      case 'D':
	for(int j = '!'; j <= '\\'; ++j){
	  transitionTable->operator[](startState)[char(j)].push_back(newState);
	  if(find(alphabet->begin(), alphabet->end(), char(j)) == alphabet->end()){
	    alphabet->push_back(char(j));
	  }
	}
	for(int j = ':'; j <= '~'; ++j){
	  transitionTable->operator[](startState)[char(j)].push_back(newState);
	  if(find(alphabet->begin(), alphabet->end(), char(j)) == alphabet->end()){
	    alphabet->push_back(char(j));
	  }
	}
	break;
      case 'W':
	for(int j = '!'; j <= '/'; ++j){
	  transitionTable->operator[](startState)[char(j)].push_back(newState);
	  if(find(alphabet->begin(), alphabet->end(), char(j)) == alphabet->end()){
	    alphabet->push_back(char(j));
	  }
	}
	for(int j = ':'; j <= '\''; ++j){
	  transitionTable->operator[](startState)[char(j)].push_back(newState);
	  if(find(alphabet->begin(), alphabet->end(), char(j)) == alphabet->end()){
	    alphabet->push_back(char(j));
	  }
	}
	for(int j = '['; j <= '^'; ++j){
	  transitionTable->operator[](startState)[char(j)].push_back(newState);
	  if(find(alphabet->begin(), alphabet->end(), char(j)) == alphabet->end()){
	    alphabet->push_back(char(j));
	  }
	}
	transitionTable->operator[](startState)[char(96)].push_back(newState);
	if(find(alphabet->begin(), alphabet->end(), char(96)) == alphabet->end()){
	  alphabet->push_back(char(96));
	}
	for(int j = '{'; j <= '~'; ++j){
	  transitionTable->operator[](startState)[char(j)].push_back(newState);
	  if(find(alphabet->begin(), alphabet->end(), char(j)) == alphabet->end()){
	    alphabet->push_back(char(j));
	  }
	}
      case 'S':
	for(int j = '!'; j <= '~'; ++j){
	  transitionTable->operator[](startState)[char(j)].push_back(newState);
	  if(find(alphabet->begin(), alphabet->end(), char(j)) == alphabet->end()){
	    alphabet->push_back(char(j));
	  }
	}
      default:
	break;
      }
    }
    //THIS MUST BE A CHARACTER ESCAPE OR A GROUP OF CHARACTERS
  }
  return newState;
}

expression expression::parse_ex(string reg){
  expression ex, second;
  string substring = "";
  for(int i = 0; i < reg.size(); ++i){
    expression sub;
    switch(reg[i]){
    case '[':
      while(reg[i] != ']'){
	sub.character += reg[i];
	++i;
      }
      sub.character += ']';
      sub.t_term = true;
      break;
    case '*':
      sub.t_star = true;
      sub.loe1.push_back(ex.loe1.back());
      ex.loe1.pop_back();
      break;
    case '|':
      sub.t_or = true;
      for(int j = 0; j < ex.loe1.size(); ++j){
	sub.loe1.push_back(ex.loe1[j]);
      }
      while(ex.loe1.size() > 0){
	ex.loe1.pop_back();
      }
      second = parse_ex(reg.substr(++i));
      for(int j = 0; j < second.loe1.size(); ++j){
	sub.loe2.push_back(second.loe1[j]);
      }
      i = reg.size();
      break;
    case '(':
      while(reg[++i] != ')'){
	substring += reg[i];
      }
      sub = parse_ex(substring);
      substring = "";
      break;
    case '\\':
      sub.t_term = true;
      sub.character += reg[i];
      sub.character += reg[++i];
      break;
    default:
      sub.character += reg[i];
      sub.t_term = true;
      break;
    }
    ex.loe1.push_back(sub);
  }
  return ex;
}
