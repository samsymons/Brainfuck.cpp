#include <iostream>
#include <string>

using namespace std;

// =================================================================
// Interpreter
// =================================================================

class BrainfuckInterpreter {

};

// =================================================================
// Argument Parsing
// =================================================================

bool has_suffix(const std::string &str, const std::string &suffix)
{
  bool size = str.size() >= suffix.size();
  return size && str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}

enum ArgumentType { Help, ExecuteFile, Unknown };

ArgumentType parse_argument(char *argument) {
  std::string help("-h");
  std::string argument_string(argument);

  if (argument_string.compare(help) == 0) {
    return Help;
  }
  else if (has_suffix(argument_string, ".b") || has_suffix(argument_string, ".bf")) {
    return ExecuteFile;
  }

  return Unknown;
}

// =================================================================
// Main
// =================================================================

void print_usage() {
  cout << "A simple Brainfuck interpreter." << endl;
  cout << "Usage: brainfuck -h | [filename]" << endl << endl;

  cout << "Arguments:" << endl;
  cout << "   [filename]            Run a Brainfuck script with a file ending in .b or .bf." << endl;
  cout << "   -h                    Get usage information." << endl;
}

bool execute_script(char *file_name) {
  cout << "Executing " << file_name << endl;

  return false;
}

int main(int argc, char **argv) {
  if (argc != 2) {
    print_usage();
    return 0;
  }

  char *command = argv[1];
  ArgumentType argument = parse_argument(command);
  
  switch (argument) {
    case ExecuteFile: {
      bool result = execute_script(command);
      return result ? 0 : 1;
    }
    default:
      print_usage();
  }

  return 0;
}
