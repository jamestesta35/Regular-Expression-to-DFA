#include <iostream>
#include <string>
#include <dirent.h>
#include "finiteMachine.h"
//#include "expression.h"

/*
  Function: help()
  Arguments: None
  Returns: None
  Description: Gives more detailed syntax and description about ls_main
*/
void help(){
  std::cout << "This is the help menu for ls_main" << std::endl;
  std::cout << "To use: $./ls_main\n";
  std::cout << "Usage: ls_main [options] [regular expression]" << std::endl;
  std::cout << "Options: -h = Help" << std::endl;
  std::cout << "         -u = Usage" << std::endl;
  std::cout << "         -v = Verbose Mode" << std::endl;
  std::cout << "              - Adds: Opening Directory, Finished Checking File" << std::endl;
  std::cout << "         -v+ = Verbose Plus Mode" << std::endl;
  std::cout << "              - Adds: All Verbose Mode functionality, Print DFA Used, Opened Directory, Closing Directory" << std::endl;
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
  Description: Gives syntax for ls_main
*/
void usage(){
  std::cout << "Usage: ls_main [options] [regular expression]" << std::endl;
}

/*
  Function: main()
  Arguments: int var_num - number of command line arguments
             char** vars - array of command line arguments
  Returns: int
  Description: Main driver for ls_main
*/
int main(int var_num, char** vars){

  //Variable declaration
  DIR *dp;
  struct dirent *ep;
  std::string reg, file, input;
  bool help_var = false;
  bool usage_var = false;
  bool verbose = false;
  bool verbosePlus = false;

  //Start of command line argument section
  for(int i = 1; i < var_num; ++i){
    if((string)vars[i] == "-h" || (string)vars[i] == "-help" || (string)vars[i] == "-H"){ //Help menu
      help_var = true;
    } else if ((string)vars[i] == "-u"){ //Usage
      usage_var = true;
    } else if ((string)vars[i] == "-v"){ //Verbose Mode
      verbose = true;
    } else if ((string)vars[i] == "-v+"){ //Verbose plus Mode
      verbose = true;
      verbosePlus = true;
    } else { //Must be regular expression
      if(reg.empty()){
	reg = string(vars[i]);
      } else {
	cout << "Unknown Variable: " << (string)vars[i] << ", Exiting!" << std::endl;
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
  //Start of ls section

  //expression express(reg);
  finiteMachine NFA(reg);
  finiteMachine DFA = NFA.toDFA();

  
  if(verbosePlus)
    DFA.printMachine();

  if(verbosePlus)
    std::cout << "Opening current directory" << std::endl;
  dp = opendir ("./");
  
  if (dp != NULL)
    {
      if(verbose)
	std::cout << "Opened directory" << std::endl;
      
      while ((ep = readdir (dp))){
	input = ep->d_name;
	
	if(verbose)
	  std::cout << "Checking file: " << input << std::endl;
	if(DFA.run(input)){
	  std::cout << input << std::endl;
	}
      }
      
      if(verbosePlus)
	std::cout << "Closing directory" << std::endl;
      (void) closedir (dp);
    }
  else{
    std::cerr << "Couldn't open the directory" << std::endl;
  }
  return 0;
}

