// =================================================================
//
// A simple interpreter for the Brainfuck language.
// Implemented based on https://en.wikipedia.org/wiki/Brainfuck.
//
// =================================================================

#include <cstdio>
#include <iostream>
#include <string>

#define BUFFER_SIZE_MAX 30000

using namespace std;

// =================================================================
// Interpreter
// =================================================================

class BrainfuckInterpreter {
  public:
    char memory[BUFFER_SIZE_MAX]; // 30K ought to be enough for anybody.
    char *data_pointer; // Point to the current location of data.
    const char *ip; // Point to the current location through the script.

    BrainfuckInterpreter(const char script_data[]) {
      data_pointer = memory;
      ip = script_data;
    }

    void increment_data_pointer() {
      data_pointer++;
    }

    void decrement_data_pointer() {
      data_pointer--;
    }

    void increment_data_byte() {
      (*data_pointer)++;
    }

    void decrement_data_byte() {
      (*data_pointer)--;
    }

    void output_byte() {
      cout << *data_pointer;
    }

    void input_byte() {
      cin >> *data_pointer;
    }

    void opening_bracket() {
      if (*data_pointer != '\0') {
        return;
      }

      int counter = 1;

      do {
        ip++;

        if (*ip == '[') {
          counter++;
        }
        else if (*ip == ']') {
          counter--;
        }
      } while (counter != 0);
    }

    void closing_bracket() {
      int counter = 0;

      do {
        if (*ip == '[') {
          counter++;
        }
        else if (*ip == ']') {
          counter--;
        }

        ip--;
      } while (counter != 0);
    }

    bool evaluate() {
      cout << "\033[32mEvaluating Brainfuck buffer with contents:" << endl << endl;
      cout << ip << endl << "\033[0m";

      while (*ip) {
        switch (*ip) {
          case '>': increment_data_pointer(); break;
          case '<': decrement_data_pointer(); break;
          case '+': increment_data_byte(); break;
          case '-': decrement_data_byte(); break;
          case '.': output_byte(); break;
          case ',': input_byte(); break;
          case '[': opening_bracket(); break;
          case ']': closing_bracket(); break;
        }
        
        ip++;
      }

      return true;
    }
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

// Execute a script with a given file name.
// 
// @return A boolean indicating whether execution was successful.
bool execute_script(char *file_name) {
  FILE *file_handle = fopen(file_name, "rb");
  if (!file_handle) {
    printf("Error: could not open file %s\n", file_name);
    return false;
  }

  char *file_contents = new char[BUFFER_SIZE_MAX];
  int result = fread(file_contents, sizeof(char), BUFFER_SIZE_MAX, file_handle);

  if (result == 0) {
    return false;
  }
  
  BrainfuckInterpreter interpreter = BrainfuckInterpreter(file_contents);
  interpreter.evaluate();

  // TODO: Return a useful value once error handling is better defined.
  return true;
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
