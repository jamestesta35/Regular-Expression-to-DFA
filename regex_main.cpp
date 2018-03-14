#include <fstream>
#include <iostream>
#include <string>
#include <dirent.h>
#include "finiteMachine.h"
#include "expression.h"

void help(){
  std::cout << "This is the help menu for regex_main" << std::endl;
}

void usage(){
  std::cout << "Usage: regex_main [options] [regular expression]" << std::endl;
}

int main(int var_num, char** vars){
  DIR *dp;
  struct dirent *ep;
  std::string regex, file, input;
  std::ifstream infile;
  int lineCount;
  bool help_var = false;
  bool usage_var = false;

  for(int i = 1; i < var_num; ++i){
    if((string)vars[i] == "-h" || (string)vars[i] == "-help" || (string)vars[i] == "-H"){
      help_var = true;
    } else if ((string)vars[i] == "-u"){
      usage_var = true;
    } else {
      if(regex.empty()){
	regex = string(vars[i]);
      } else {
	cout << "Too many variables. Exiting." << std::endl;
	return 2;
      }
    }
  }

  if(usage_var){
    usage();
  }

  if(help_var){
    help();
  }

  if(usage_var || help_var){
    return 1;
  }

  if(regex.empty()){
    std::cout << "Expected a regular expression" << endl;
    return 3;
  }
  
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
