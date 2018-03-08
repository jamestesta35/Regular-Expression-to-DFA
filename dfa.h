#include <map>
#include <string>
#include <vector>

using namespace std;

class dfa {
 public:
  dfa();
  dfa(vector<string> states, vector<char> alphabet, map<string, map<char, string> > transitionTable, string startState, vector<string> endStates);
  bool checkInput(string input);
 private:
  vector<string> states_;
  vector<char> alphabet_;
  map<string, map<char, string> > transitionTable_;
  string startState_;
  vector<string> endStates_;
};
