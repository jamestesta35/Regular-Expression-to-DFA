#include <iostream>
#include <string>
#include <dirent.h>
#include "finiteMachine.h"
#include "expression.h"

int main(){
  DIR *dp;
  struct dirent *ep;
  std::string regex, file, input;
  std::cout << "Type the regular expression:" << endl;
  std::cin >> regex;
  regex = ".*" + regex + ".*";
  expression express(regex);
  finiteMachine NFA(regex);
  finiteMachine DFA = NFA.toDFA();
  
  dp = opendir ("./");
  if (dp != NULL)
    {
      while ((ep = readdir (dp))){
	input = ep->d_name;
	if(DFA.run(input)){
	  std::cout << input << std::endl;
	}
      }
      (void) closedir (dp);
    }
  else{
    std::cerr << "Couldn't open the directory" << std::endl;
  }
  return 0;
}

