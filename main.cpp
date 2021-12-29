#include "include/main.h"
#include <filesystem>

using namespace std;
void REX::printUsage() {
  char esc_char = 27;
  cout << "\n"
       << esc_char << "[1m"
       << "                         WELCOME TO REX!               " << esc_char
       << "[0m"
       << "\n\n";
  cout << " Functionalities offered: \n";
  cout << "  - Find all the occurrences of a certain word or a sentence in the "
          "selected codebase. \n";
  cout << "  - Replace all the occurrences of a certain word or a sentence in "
          "the selected codebase by a something else. \n";
  cout << "  - Revert the replacement changes done. \n";
  cout << "\n Usage: \n";
  cout << "  - For finding all/some occurrences: \n";
  cout << "       => rex [directory path('.' or 'full path')] --find [word or "
          "sentence] --all(for the whole directory)/--some [filePath1] "
          "[filePath2] ... \n\n";
  cout << "  - For replacing all/some occurrences: \n";
  cout << "       => rex [directory path('.' or 'full path')] --replace [word "
          "or sentence to be replaced] [word or sentence to be replaced with] "
          "--all(for the whole directory)/--some [filePath1] [filePath2] ... "
          "\n\n";
  cout << "  - For reverting the changes made by the replace operation: \n";
  cout << "       => rex [directory path('.' or 'full path')] --revert [unique "
          "code appended after '.' in the log file] \n\n";
  cout << "\n Terminologies: \n";
  cout << "   --find      operation specification flag, used to find code "
          "snippets in the current working directory. \n";
  cout << "   --replace   operation specification flag, used to replace code "
          "snippets with the provided ones in the current working directory.\n";
  cout << "   --revert    operation specification flag, used to revert the "
          "changes made by the replacement operation.\n";
  cout << "   --all       flag for considering every file in the current "
          "working directory.\n";
  cout << "   --some      flag for considering some selected few files in the "
          "current working directory.\n\n";
  cout << "   --display     flag for displaying the details of the operations "
          "being performed.\n";
  cout << "   --include-only     flag for only considering certain file "
          "types.\n";
  cout << "   --except     flag for excluding certain file types.\n";
  cout << "\n\n WARNING: \n";
  cout
      << "     '--revert' will only work if the files in which the changes via "
         "the replacement operation were made haven't changed.\n\n";
  //   cout << "\n TIP: \n     Just append '--display' flag at the end of your
  //   command to display the operation details.\n"     To perform any operation
  //   on a certain file type, consider using '--include-only' flag, just
  //       add it as a suffix to the whole command,
  //       like so : [whole command]-- include -
  //                 only cpp java py...\n To exclude any file(s)
  //                     from the operation being performed,
  //       just add the flag '--except' and the file types,
  //       like so[whole command]-- except cpp java py... ";
}

REX::REX(char *arguments[], int const &size) {
  this->cur_op = operation();
  this->_parse(arguments, size);
  this->cur_op.show();
}

void REX::_parse(char *arguments[], int const &size) {
  string path, target, replacer, rest;
  operation_type op_type;
  operation_scope op_scope;
  vector<string> filepaths;
  operation_target op_target;
  vector<pair<string, operation_target>> special_files;
  bool is_target_provided = false;
  for (int i = 1; i < size; i++) {
    string argument(arguments[i]);
    switch (i) {
    case 1: {
      path = argument;
      break;
    }
    case 2: {
      if (argument == "--find") {
        op_type = operation_type::find;
      } else if (argument == "--replace") {
        op_type = operation_type::replace;
      } else if (argument == "--revert") {
        op_type = operation_type::revert;
      } else {
        throw runtime_error("Unknown operation type: " + string(argument));
      }
      break;
    }
    case 3: {
      target = argument;
      break;
    }
    case 4: {
      if (argument == "--all") {
        op_scope = operation_scope::all;
      } else if (argument == "--some") {
        op_scope = operation_scope::some;
      } else {
        throw runtime_error("Unknown operation scope: " + string(argument));
      }
      break;
    }
    case 5: {
      if (op_scope == operation_scope::some) {
        string resolve = argument;
        while (utility::getsubstr(resolve, 0, 1) != "--" && i < size) {
          if (arguments[i] == "" || arguments[i] == " ")
            break;
          filepaths.push_back(arguments[i++]);
          resolve = string(arguments[i]);
        }
        if (filepaths.empty()) {
          throw runtime_error("Missing files");
        }
        i--;
        break;
      }
    }
    default: {
      if (is_target_provided) {
        special_files.push_back({argument, op_target});
        continue;
      }
      if (argument == "--include-only") {
        op_target = operation_target::includeOnly;
        is_target_provided = true;
      } else if (argument == "--except") {
        op_target = operation_target::excludeOnly;
        is_target_provided = true;
      } else {
        rest += argument + ' ';
      }
    }
    }
  }
  if (op_scope == operation_scope::some) {
    if (filepaths.empty()) {
      throw runtime_error("Missing files");
    }
  }
  this->cur_op.reinitialize(path, target, op_type, op_scope, rest, replacer,
                            filepaths, special_files);
}

void REX::_register(std::string const &filepath) {
  for (auto &file : std::filesystem::recursive_directory_iterator(filepath)) {
    cout << file.path() << "\n";
  }
}

int main(int argc, char *argv[]) {
  if (argc < 5) {
    REX::printUsage();
    return 0;
  }
  try {
    REX instance(argv, argc);
  } catch (std::exception &e) {
    cout << "ERROR: " << e.what();
  }
  return cout << "\n", 0;
}
