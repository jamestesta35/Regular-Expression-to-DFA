#include <fstream>
#include <iostream>
#include <string>
#include <dirent.h>
#include "finiteMachine.h"
#include "expression.h"

int main(){
  DIR *dp;
  struct dirent *ep;
  std::string regex, file, input;
  std::ifstream infile;
  int lineCount;
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
	file = ep->d_name;
	infile.open(file);
	if(infile){
	  lineCount = 1;
	  while (getline( infile, input )){
	    if(DFA.run(input)){
	      std::cout << file << ":" << lineCount << ":" << input << std::endl;
	    }
	    ++lineCount;
	  }
	} else {
	  std::cout << "Warning: Unable to open " << file << " for reading. Unable to check for regex string." << endl;
	}
	infile.close();
      }
      (void) closedir (dp);
    }
  else{
    std::cerr << "Couldn't open the directory" << std::endl;
  }
  
  return 0;
}
