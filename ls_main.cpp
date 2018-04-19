#include <iostream>
#include <string>
#include <dirent.h>
#include "finiteMachine.h"

/*
  Function: usege()
  Arguments: None
  Returns: None
  Description: Gives syntax for ls_main
*/
void usage(){
  std::cout << "Usage: ls_main [options] [Regular Expression]" << std::endl;
}

/*
  Function: help()
  Arguments: None
  Returns: None
  Description: Gives more detailed syntax and description about ls_main
*/
void help(){
  std::cout << "This is the help menu for regex_main" << std::endl;
  std::cout << "To use: $./regex_main\n";
  usage();
  std::cout << "Options: -d = Directory" << std::endl;
  std::cout << "              - The next argument will set the new directory pointer" << std::endl;
  std::cout << "         -e = Exact" << std::endl;
  std::cout << "              - The entire line must match the regular expression exactly" << std::endl;
  std::cout << "         -f = File" << std::endl;
  std::cout << "              - The next argument will be the specific file to check" << std::endl;
  std::cout << "         -l = Load Finite Machine" << std::endl;
  std::cout << "              - The next argument will be the specific json file to load a DFA or NFA" << std::endl;
  std::cout << "         -h = Help" << std::endl;
  std::cout << "         -u = Usage" << std::endl;
  std::cout << "         -v = Verbose Mode" << std::endl;
  std::cout << "              - Adds: Prints DFA" << std::endl;
  std::cout << "         -dfa = Print DFA" << std::endl;
  std::cout << "              - The next argument will be the specific file to print the DFA used for the regular expression" << std::endl;
  std::cout << "         -nfa = Print NFA" << std::endl;
  std::cout << "              - The next argument will be the specific file to print the NFA used for the regular expression" << std::endl;
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
  std::string regex, file = "", reg, directory = "./", print_dfa = "", print_nfa = "", load = "";
  bool exact = false;
  bool help_var = false;
  bool usage_var = false;
  bool verbose = false;

  //Start of command line argument section
  for(int i = 1; i < var_num; ++i){
    if((string)vars[i] == "-h" || (string)vars[i] == "-help" || (string)vars[i] == "-H"){ //Help menu
      help_var = true;
    } else if ((string)vars[i] == "-u"){ //Usage
      usage_var = true;
    } else if ((string)vars[i] == "-v"){ //Verbose Mode
      verbose = true;
    } else if ((string)vars[i] == "-e"){ //Exact
      exact = true;
    } else if ((string)vars[i] == "-nfa"){ //Print NFA
      if(++i >= var_num){
	std::cout << "Must provide a file after -nfa" << std::endl;;
      }
      print_nfa = (string)vars[i];
    } else if ((string)vars[i] == "-dfa"){ //Print DFA
      if(++i >= var_num){
	std::cout << "Must provide a file after -dfa" << std::endl;;
      }
      print_dfa = (string)vars[i];
    }else if ((string)vars[i] == "-l"){ //Load json
      if(++i >= var_num){
	std::cout << "Must provide a file after -l" << std::endl;;
      }
      load = (string)vars[i];
    }else if ((string)vars[i] == "-f"){ //Check Specific File
      if(++i >= var_num){
	std::cout << "Must provide a file after -f" << std::endl;;
      }
      file = (string)vars[i];
    } else if ((string)vars[i] == "-d"){ //Specific Directory
      if(++i >= var_num){
	std::cout << "Must provide a directory after -d" << std::endl;;
      }
      directory = (string)vars[i];
    } else { //Must be regular expression
      if(reg.empty()){
	reg = string(vars[i]);
      } else {
	std::cout << "Unknown Variable: " << (string)vars[i] << ", Exiting!" << std::endl;
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
  if(reg.empty() && load == ""){
    std::cout << "Expected a regular expression" << std::endl;
    return 3;
  }
  //End of the command line arugment section
  //Start of regex section

  //Regex will search for sequences inside of lines so add .* so check inside the line
  if(!exact && load == ""){
    regex = ".*(" + reg + ").*";
  } else {
    regex = reg;
  }
  
  //Create the DFA
  //Regular Expression -> NFA -> DFA
  finiteMachine NFA;
  if(load != ""){
    if(NFA.loadJson(load)){
      cout << "Unable to load Json finite machine from file.\n";
      return 12;
    } else {
      //cout << "LOADED FROM FILE\n";
    }
  } else {
    NFA = finiteMachine(regex);
  }
  if(print_nfa != ""){
    if(NFA.printJson(print_nfa)){
      cout << "Unable to print NFA to file.\n";
      return 10;
    }
  }
  
  finiteMachine DFA = NFA.toDFA();
  if(print_dfa != ""){
    if(DFA.printJson(print_dfa)){
      cout << "Unable to print DFA to file.\n";
      return 11;
    }
  }
    
  if(verbose)
    DFA.printMachine();
  //Run through all files in the current directory
  if(file != ""){
    if(DFA.run(file)){
      std::cout << file << "\n";
    }
  } else {
    const char *direct = directory.c_str();
    dp = opendir (direct);
    if (dp != NULL)
      {
	while ((ep = readdir (dp))){
	  file = ep->d_name;
	  if(DFA.run(file))
	    cout << file << "\n";
	}
	(void) closedir (dp);
      }
    else{
      std::cerr << "Couldn't open the directory" << std::endl;
    }
  }
  
  return 0;
}
