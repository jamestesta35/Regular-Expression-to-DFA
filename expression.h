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
  expression();
  expression(string regex);
  string toNFA(string startState, vector<string>* states, map<string, map<char, vector<string> > >* transitionTable, vector<char>* alphabet);

 private:
  string termToNFA(string character, string startState, vector<string>* states, map<string, map<char, vector<string> > >* transitionTable, vector<char>* alphabet);
  string orToNFA(vector<expression> loe1, vector<expression> loe2, string startState, vector<string>* states, map<string, map<char, vector<string> > >* transitionTable, vector<char>* alphabet);
  string starToNFA(vector<expression> loe1, string startState, vector<string>* states, map<string, map<char, vector <string> > >* transitionTable, vector<char>* alphabet);
  expression parse_ex(string regex);
};
