#include <map>
#include <string>
#include <vector>

using namespace std;

class expression{
public:
  bool t_star;
  bool t_or;
  bool t_term;
  string character; // term
  vector<expression> loe1; // or, star, or none
  vector<expression> loe2;// or
/*
  Function: expression()
  Arguments: None
  Returns: None
  Description: Basic constructor for expression
*/
  expression();
/*
  Function: expression()
  Arguments: string regex - Regular Expression to use
  Returns: None
  Description: Constructor for expression based on regular expression
*/
  expression(string regex);
/*
  Function: toNFA()
  Arguments: string startState
             vector<string>* states
	     map<string, map<char
	     vector<string> > >* transitionTable
	     vector<char>* alphabet
  Returns: string - the final state node of the expression
  Description: Converts the variables to a NFA based on the expression stored
*/
  string toNFA(string startState, vector<string>* states, map<string, map<char, vector<string> > >* transitionTable, vector<char>* alphabet);

 private:
/*
  Function: termToNFA()
  Arguments: string character
             string startState
             vector<string>* states
	     map<string, map<char
	     vector<string> > >* transitionTable
	     vector<char>* alphabet
  Returns: string - the final state node of the expression
  Description: Converts the expression segment to a term of a finite machine
*/
  string termToNFA(string character, string startState, vector<string>* states, map<string, map<char, vector<string> > >* transitionTable, vector<char>* alphabet);
  
/*
  Function: orToNFA()
  Arguments: vector<expression> loe1
             vector<expression> loe2
             string startState
             vector<string>* states
	     map<string, map<char
	     vector<string> > >* transitionTable
	     vector<char>* alphabet
  Returns: string - the final state node of the expression
  Description: Converts the expression segment to a or segment of a finite machine
*/
  string orToNFA(vector<expression> loe1, vector<expression> loe2, string startState, vector<string>* states, map<string, map<char, vector<string> > >* transitionTable, vector<char>* alphabet);

  /*
  Function: starToNFA()
  Arguments: vector<expression> loe1
             string startState
             vector<string>* states
	     map<string, map<char
	     vector<string> > >* transitionTable
	     vector<char>* alphabet
  Returns: string - the final state node of the expression
  Description: Converts the expression segment to a star segment of a finite machine
*/
  string starToNFA(vector<expression> loe1, string startState, vector<string>* states, map<string, map<char, vector <string> > >* transitionTable, vector<char>* alphabet);

  /*
  Function: parse_ex()
  Arguments: string regex
  Returns: expression - a new parsed expression
  Description: Converts an expression into a parsed expression
*/
  expression parse_ex(string regex);
};
