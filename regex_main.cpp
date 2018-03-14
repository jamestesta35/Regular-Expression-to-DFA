#include <fstream>
#include <iostream>
#include <string>
#include <dirent.h>
#include "finiteMachine.h"
#include "expression.h"

/*
  Function: help()
  Arguments: None
  Returns: None
  Description: Gives more detailed syntax and description about regex_main
*/
void help(){
  std::cout << "This is the help menu for regex_main" << std::endl;
  std::cout << "To use: $./regex_main\n";
  std::cout << "Usage: regex_main [options] [regular expression]" << std::endl;
  std::cout << "Options: -h = Help" << std::endl;
  std::cout << "         -u = Usage" << std::endl;
  std::cout << "Regular Expression:" << std::endl;
  std::cout << "\tCharacters: ascii characters 'space' -> '~'\n";
  std::cout << "\t            \\d: One digit from 0-9\n";
  std::cout << "\t            \\w: One word character (letter, digit, underscore)\n";
  std::cout << "\t            \\s: One whitespace character (space, tab, newline, carriage return, vertical tab)\n";
  std::cout << "\t            \\D: One character that is not a digit\n";
  std::cout << "\t            \\W: One character that is not a word character\n";
  std::cout << "\t            \\S: One character that is not a whitespace character\n";
  std::cout << "\t            . : Wild card (space -> ~)\n";
  std::cout << "\tSequences:  [...]: Any character in the [] ([abc], [3d8])\n";
  std::cout << "\t            [.-.]: Any character between the two ascii characters ([a-z], [0-9])\n";
  std::cout << "\tOther:      *    : 0 or more times applied to the previous group or character\n";
  std::cout << "\t            (...): Capture Group to group sequences of characters/strings\n";
  std::cout << "\tNote: You can concatinate these characters/sequences by typing them next to each other in a string format\n";
  std::cout << std::endl;
}

/*
  Function: usege()
  Arguments: None
  Returns: None
  Description: Gives syntax for regex_main
*/
void usage(){
  std::cout << "Usage: regex_main [options] [regular expression]" << std::endl;
}

/*
  Function: main()
  Arguments: None
  Returns: int
  Description: Main driver for regex_main
*/
int main(int var_num, char** vars){

  //Variable declaration
  DIR *dp;
  struct dirent *ep;
  std::string regex, file, input, reg;
  std::ifstream infile;
  int lineCount, total = 0;
  bool help_var = false;
  bool usage_var = false;

  //Start of command line argument section
  for(int i = 1; i < var_num; ++i){
    if((string)vars[i] == "-h" || (string)vars[i] == "-help" || (string)vars[i] == "-H"){ //Help menu
      help_var = true;
    } else if ((string)vars[i] == "-u"){ //Usage
      usage_var = true;
    } else { //Must be regular expression
      if(reg.empty()){
	reg = string(vars[i]);
      } else {
	cout << "Too many variables. Exiting." << std::endl;
	return 2;
      }
    }
  }
  //Show usage
  if(usage_var){
    usage();
  }
  //Show help menu
  if(help_var){
    help();
  }
  //End if help or usage is shown
  if(usage_var || help_var){
    return 1;
  }
  //End if 
  if(reg.empty()){
    std::cout << "Expected a regular expression" << endl;
    return 3;
  }
  //End of the command line arugment section
  //Start of regex section

  //Regex will search for sequences inside of lines so add .* so check inside the line
  regex = ".*" + reg + ".*";
  
  //Create the DFA
  //Regular Expression -> NFA -> DFA
  expression express(regex);
  finiteMachine NFA(regex);
  finiteMachine DFA = NFA.toDFA();

  //Run through all files in the current directory
  dp = opendir ("./");
  if (dp != NULL)
    {
      while ((ep = readdir (dp))){
	file = ep->d_name;
	infile.open(file);
	if(infile){
	  lineCount = 1;
	  //Run through each line for the current file
	  while (getline( infile, input )){
	    //If the line contains the regular expression print it to screen
	    if(DFA.run(input)){
	      std::cout << file << ":" << lineCount << ":" << input << std::endl;
	      ++total;
	    }
	    ++lineCount;
	  }
	} else {
	  std::cout << "WARNING: Unable to open " << file << " for reading. Unable to check for regex string." << endl;
	}
	infile.close();
      }
      (void) closedir (dp);
    }
  else{
    std::cerr << "Couldn't open the directory" << std::endl;
  }
  //Final output for the regex_main
  std::cout << "Found " << total << " results for: " << reg << std::endl;
  
  return 0;
}
