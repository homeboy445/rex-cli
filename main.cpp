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
  cout << "\n   --include-only     flag for only considering certain file"
          "types(specify file types only without a period).\n";
  cout << "   --except     flag for excluding certain file types(specify file "
          "types only without a period).\n";
  cout << "\n\n WARNING: \n";
  cout
      << "     '--revert' will only work if the files in which the changes via "
         "the replacement operation were made haven't changed.\n\n";
  cout << "\n TIP: \n     * Just append '--display' flag at the end of your "
       << "command to display the operation details.\n     * To perform any "
          "operation "
       << "on a certain file type, consider using '--include-only' flag, just"
       << "add it as a suffix to the whole command,"
       << "like so : [whole command]-- include -"
       << "only cpp java py...\n     * To exclude any file(s) from the "
          "operation "
          "being performed,"
       << "just add the flag '--except' and the file types, like "
          "so[whole "
          "command]-- except cpp java py... \n"
       << "     * By default the operation scope would be '--all', unless "
          "specified "
          "otherwise.\n\n";
}

REX::REX(char *arguments[], int const &size) {
  this->start = std::chrono::steady_clock::now();
  this->cur_op = operation();
  this->_parse(arguments, size);
  // this->cur_op.show();
  this->_register(arguments[1]);
  this->_initiate_operation();
  this->end = std::chrono::steady_clock::now();
  cout << "\nThe operation took: " << this->getElapsedTime() << "ms\n";
}

int REX::getElapsedTime() {
  return std::chrono::duration_cast<std::chrono::milliseconds>(this->end -
                                                               this->start)
      .count();
}

void REX::_parse(char *arguments[], int const &size) {
  string path, target, replacer, rest;
  operation_type op_type;
  operation_scope op_scope;
  operation_target op_target;
  int lines = 0;
  vector<pair<string, operation_target>> special_files;
  bool is_target_provided = false;
  for (int i = 1; i < size; i++) {
    string argument(arguments[i]);
    this->unique_identifier += argument;
    switch (i) {
    case 1: {
      if (argument == ".") {
        path = utility::getcwd_string();
      } else {
        path = argument;
      }
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
      op_scope = operation_scope::all;
      break;
    }
    case 4: {
      if (argument == "--all") {
        op_scope = operation_scope::all;
      } else if (argument == "--some") {
        op_scope = operation_scope::some;
      } else {
        if (op_scope == operation_scope::all && argument == "--include-only" ||
            argument == "--except" || argument == "--lines") {
          goto default_;
        }
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
          this->filepaths[string(arguments[i++])] = true;
          if (i >= size)
            break;
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
    default_:
      if (argument == "--lines") {
        lines = atoi(arguments[++i]);
        break;
      }
      if (is_target_provided) {
        special_files.push_back({argument, op_target});
        break;
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
  this->cur_op.reinitialize(path, target, op_type, op_scope, lines, rest,
                            replacer, special_files);
}

void REX::_register(std::string const &filepath) {
  unordered_map<string, operation_target> inc_ex;
  bool includeOnly = false;
  for (auto const &f_type : this->cur_op.get_specialfiles()) {
    includeOnly |= f_type.second == operation_target::includeOnly;
    inc_ex[f_type.first] = f_type.second;
  }
  for (auto &file : std::filesystem::recursive_directory_iterator(filepath)) {
    string fpath = file.path();
    string type = utility::getfiletype(fpath);
    if (!utility::isfiletypevalid(type) || type == "o" || type == "" ||
        type == "out" || type == "invalid!" || type == ".exe")
      continue;
    if (includeOnly) {
      if (inc_ex.find(type) == inc_ex.end() ||
          inc_ex[type] != operation_target::includeOnly)
        continue;
    } else {
      if (inc_ex[type] == operation_target::excludeOnly)
        continue;
    }
    this->file_instances.push_back(code_helper(fpath));
  }
}

void REX::_initiate_operation() {
  switch (this->cur_op.get_operationtype()) {
  case operation_type::find: {
    this->_find();
    break;
  }
  default:
    break;
  }
}

void REX::_find() {
  auto print_results = [&](string const &target) -> void {
    cout << "Total match found: "
         << double(double(this->found_results.size()) + 0.0) << "\n";
    for (auto &result : this->found_results) {
      cout << (this->cur_op.getlines() == 0 ? "" : "\n -----\n");
      cout << "\nfile: " << result.first << ",\n";
      vector<string> results_ = utility::split(result.second.second + '\n');
      for (int i = 0; i < results_.size(); i++) {
        if (i == this->cur_op.getlines()) {
          cout << "At line no. " << result.second.first + 1
               << ": " // TODO: You could add another option to avoid colorising
                       // the results, which might lead to performance being
                       // optimized.
               << utility::colorise_occurences(results_[i], target) << "\n";
        } else {
          cout << "                " << results_[i] << "\n";
        }
      }
      cout << (this->cur_op.getlines() == 0 ? "" : "\n xxxxx \n");
    }
  };
  switch (this->cur_op.get_operationscope()) {
  case operation_scope::all: {
    const auto [target, replacer] = this->cur_op.get_targetAndreplacer();
    if (this->cached_results.find(this->unique_identifier) !=
        this->cached_results
            .end()) { // This might prove to be a redundant piece of code ;( .
      this->found_results = this->cached_results[this->unique_identifier];
    } else {
      for (auto &code : this->file_instances) {
        for (auto result : code.find(target, this->cur_op.getlines())) {
          this->found_results.push_back({code.getpath(), result});
        }
      }
    }
    // system("clear");
    print_results(target);
    break;
  }
  case operation_scope::some: {
    const auto [target, replacer] = this->cur_op.get_targetAndreplacer();
    if (this->cached_results.find(this->unique_identifier) !=
        this->cached_results
            .end()) { // This might prove to be a redundant piece of code ;( .
      this->found_results = this->cached_results[this->unique_identifier];
    } else {
      for (auto &code : this->file_instances) {
        for (auto result : code.find(target)) {
          if (this->filepaths.find(code.getpath()) != this->filepaths.end()) {
            this->found_results.push_back({code.getpath(), result});
          }
        }
      }
    }
    // system("clear");
    print_results(target);
    break;
  }
  default: {
    throw runtime_error("Unknown operation scope!");
  }
  }
}

int main(int argc, char *argv[]) {
  if (argc < 4) {
    REX::printUsage();
    return 0;
  }
  try {
    REX instance(argv, argc);
  } catch (std::exception &e) {
    cout << "\nERROR: " << e.what() << "\n\n";
    REX::printUsage();
  }
  return cout << "\n", 0;
}
