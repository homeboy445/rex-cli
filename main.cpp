#include "include/main.h"
#include <filesystem>

using namespace std;
namespace util {
void promptLogPathLocation(operation &curOp) {
  cout << "Location for saving operation log file(default location would the "
          "pwd): ";
  cin >> curOp.logFilePath;
  cout << "\n";
}
class console_ {
public:
  /**
   * @brief A multi-argument accepting method which act's as JavaScript's
   * console.log function.
   *
   * @tparam Args
   * @param args
   */
  template <class... Args> void log(Args... args) {
    (std::cout << ... << args) << "\n";
  }
};
console_ console;
unordered_map<string, bool> _keywords_ = {
    {"--some", true},   {"--include-only", true}, {"--some", true},
    {"--except", true}, {"--find", true},         {"--replace", true},
    {"--skip", true},   {"--revert", true}};
}; // namespace util
using namespace util;

void REX::printUsage() {
  char esc_char = 27;
  cout << "\n"
       << esc_char << "[1m"
       << "                         WELCOME TO REX!               " << esc_char
       << "[0m"
       << "\n\n";
  cout << " Functionalities offered: \n";
  cout
      << "  - Find some the occurrences of a certain word or a sentence in the "
         "selected codebase. \n";
  cout << "  - Replace some the occurrences of a certain word or a sentence in "
          "the selected codebase by a something else. \n";
  cout << "\n Usage: \n";
  cout << "  - For finding some/some occurrences: \n";
  cout << "       => rex [directory path('.' or 'full path')] --find [word or "
          "sentence] --some(for the whole directory)/--some [filePath1] "
          "[filePath2] ... \n\n";
  cout << "  - For replacing some/some occurrences: \n";
  cout << "       => rex [directory path('.' or 'full path')] --replace [word "
          "or sentence to be replaced] [word or sentence to be replaced with] "
          "--some(for the whole directory)/--some [filePath1] [filePath2] ... "
          "\n\n";
  cout << "\n Flags(actual precedence order): \n";
  cout << "   --find      operation specification flag, used to find code "
          "snippets in the current working directory. \n";
  cout << "   --replace   operation specification flag, used to replace code "
          "snippets with the provided ones in the current working directory.\n";
  cout << "   --list     if multiple words are to be found, present this flag "
          "and pass the keywords in a listing fashion.\n";
  cout << "   --some       flag for considering every file in the current "
          "working directory(default).\n";
  cout << "   --some      flag for considering some selected few files in the "
          "current working directory.\n";
  cout << "   --lines     flag for displaying more than one line in the result "
          "output[just provide the integer after it].\n";
  cout << "   --include-only     flag for only considering certain file"
          "types(specify file types only without a period).\n";
  cout << "   --except     flag for excluding certain file types(specify file "
          "types only without a period).\n";
  cout << "\n NOTE: Currently only these file types are supported: c, cpp, js, "
          "mjs, ts, java, py, txt.\n";
  cout << "\n TIP: \n     * Just append '--display' flag at the end of your "
       << "command to display the operation details(in the case of replace "
          "operation).\n     * To perform any "
          "operation "
       << "on a certain file type, consider using '--include-only' flag, just"
       << " add it as a suffix to the whole command,"
       << "like so : [whole command] --include-"
       << "only cpp java py...\n     * To exclude any file(s) from the "
          "operation "
          "being performed,"
       << "just add the flag '--except' and the file types, like "
          "so: [whole "
          "command] --except cpp java py... \n"
       << "     * By default the operation scope would be '--some', unless "
          "specified "
          "otherwise.\n\n";
}

REX::REX(char *arguments[], int const &size) : currentOperation() {
  this->parse(arguments, size);
  if (this->currentOperation.get_operationtype() == operation_type::replace) {
    // promptLogPathLocation(this->currentOperation);
  }
}

void REX::run() {
  this->start = std::chrono::steady_clock::now();
  this->_register(this->currentOperation.getFilePath());
  this->initiateOperation();
  this->end = std::chrono::steady_clock::now();
  cout << "\nThe operation took: " << this->getElapsedTime() << "ms\n";
}

int REX::getElapsedTime() {
  return std::chrono::duration_cast<std::chrono::milliseconds>(this->end -
                                                               this->start)
      .count();
}

void REX::parse(char *arguments[], int const &size) {
  string path, target, replacer, rest;
  operation_type op_type;
  operation_scope op_scope;
  operation_target op_target;
  int lines = 0;
  vector<pair<string, operation_target>> special_files;
  bool is_target_provided = false;
  if (string(arguments[1]).compare("--revert") == 0) {
    op_type = operation_type::revert;
    op_scope = operation_scope::some;
    target = arguments[2];
    goto done;
  }
  for (int i = 1, case_ = 1; i < size; i++, case_++) {
    string argument(arguments[i]);
    this->unique_identifier += argument;
    switch (case_) {
    case 1: { // TODO: THIS IS REDUNDANT... REMOVE IT...
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
      } else {
        throw runtime_error("Unknown operation type: " + string(argument));
      }
      break;
    }
    case 3: {
      if (argument == "--list") { // Add support for this...
        throw runtime_error("Currently not supported!");
        int j = i + 1;
        while (j < size && util::_keywords_.find(string(arguments[j])) ==
                               util::_keywords_.end()) {
          target += string(arguments[j++]) + '\n';
        }
        i = j - 1;
        op_scope = operation_scope::some;
        break;
      }
      target = argument;
      op_scope = operation_scope::some;
      if (op_type == operation_type::replace) {
        if (i + 1 >= size)
          throw runtime_error(
              "Replacer target not found! [error while parsing arguments]");
        replacer = string(arguments[++i]);
      }
      break;
    }
    case 4: {
      if (argument == "--some") {
        op_scope = operation_scope::some;
      } else if (argument == "--some") {
        op_scope = operation_scope::some;
      } else {
        if (op_scope == operation_scope::some && argument == "--include-only" ||
            argument == "--except" || argument == "--lines") {
          goto DEFAULT;
        }
        throw runtime_error("Unknown operation scope: " + argument +
                            " [error while parsing arguments]");
      }
      break;
    }
    case 5: {
      if (op_scope == operation_scope::some) {
        string resolve = argument;
        while (utility::getsubstr(resolve, 0, 1) != "--" && i < size) {
          if (arguments[i] == "" || arguments[i] == " ")
            break;
          this->filepaths[utility::validate_path(string(arguments[i++]))] =
              true;
          if (i >= size)
            break;
          resolve = string(arguments[i]);
        }
        if (filepaths.empty()) {
          throw runtime_error("Missing files! [error while parsing arguments]");
        }
        i--;
        break;
      }
    }
    default: {
    DEFAULT:
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
done:
  if (op_scope == operation_scope::some) {
    if (filepaths.empty()) {
      throw runtime_error("Missing files! [error while parsing arguments]");
    }
  }
  this->currentOperation.reinitialize(path, target, op_type, op_scope, lines,
                                      rest, replacer, special_files);
}

void REX::_register(std::string const &filepath) {
  if (this->currentOperation.get_operationtype() == operation_type::revert) {
    return;
  }
  unordered_map<string, operation_target> targetFiles;
  bool includeOnly = false;
  for (auto const &f_type : this->currentOperation.get_specialfiles()) {
    includeOnly |= f_type.second == operation_target::includeOnly;
    targetFiles[f_type.first] = f_type.second;
  }
  if (filepath.compare(".") == 0) {
    throw runtime_error("Error invalid file path!");
    return;
  }
  for (auto &file : std::filesystem::recursive_directory_iterator(filepath)) {
    string fpath = file.path();
    string type = utility::getfiletype(fpath);
    if (!(type == "cpp" || type == "c" || type == "java" || type == "py" ||
          type == "js" || type == "ts" || type == "mjs" ||
          type == "txt")) { // FIXME: Extend the file types...!
      continue;
    }
    if (includeOnly) {
      if (targetFiles.find(type) == targetFiles.end() ||
          targetFiles[type] != operation_target::includeOnly) {
        continue;
      }
    } else {
      if (targetFiles[type] == operation_target::excludeOnly) {
        continue;
      }
    }
    this->file_instances.push_back(code_helper(fpath));
  }
}

void REX::initiateOperation() {
  switch (this->currentOperation.get_operationtype()) {
  case operation_type::find: {
    this->find();
    break;
  }
  case operation_type::replace: {
    this->replace();
    break;
  }
  case operation_type::revert: {
    this->revert();
    break;
  }
  default:
    break;
  }
}

void REX::find() { // TODO: Consider using threads to make this cli a
                   // non-blocking one.
  const auto [target, replacer] =
      this->currentOperation.get_targetAndreplacer();
  auto print_results = [&](string const &target) -> void {
    cout << "Total match found: "
         << double(double(this->found_results.size()) + 0.0) << "\n";
    for (auto &result : this->found_results) {
      cout << (this->currentOperation.getlines() == 0 ? "" : "\n -----\n");
      cout << "\nfile: " << result.first << ",\n";
      vector<string> results_ = utility::split(result.second.second + '\n');
      for (int i = 0; i < results_.size(); i++) {
        if (i == this->currentOperation.getlines()) {
          cout << "At line no. " << result.second.first + 1
               << ": " /* TODO: You could add another option to avoid colorising
                      the results, which might lead to performance being
                      optimized. */
               << utility::colorise_occurences(results_[i], target) << "\n";
        } else {
          cout << "                " << results_[i] << "\n";
        }
      }
      cout << (this->currentOperation.getlines() == 0 ? "" : "\n xxxxx \n");
    }
  };
  switch (this->currentOperation.get_operationscope()) {
  case operation_scope::some: {
    for (auto &code : this->file_instances) {
      for (auto result : code.find(target, this->currentOperation.getlines())) {
        this->found_results.push_back({code.getpath(), result});
      }
    }
    // system("clear");
    print_results(target);
    break;
  }
  case operation_scope::some: {
    for (auto &code : this->file_instances) {
      for (auto result : code.find(target)) {
        if (this->filepaths.find(code.getpath()) != this->filepaths.end()) {
          this->found_results.push_back({code.getpath(), result});
        } // TODO: Shorten this code, take inspiration from the below code for
          // replace method ;D.
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

void REX::replace() {
  const auto [target, replacer] =
      this->currentOperation.get_targetAndreplacer();
  vector<pair<string, pair<int, pair<string, string>>>> results_;
  if (target == replacer) {
    cout << "target and replacer are same... Haulting execution...!\n";
    exit(0);
  }
  auto print_results = [&]() {
    string log_data;
    cout << "Total results found: " << results_.size() << "\n";
    log_data += "Total results found: " + to_string(results_.size()) + "\n";
    for (auto const &result_ : results_) {
      cout << "file: " << result_.first << ", At line no. "
           << result_.second.first << ",\n";
      log_data += "file: " + result_.first + ", At line no. " +
                  to_string(result_.second.first) + ",\n";
      cout << "From: "
           << utility::colorise_occurences(result_.second.second.first, target)
           << "\n";
      cout << "To: "
           << utility::colorise_occurences(result_.second.second.second,
                                           replacer)
           << "\n\n";
      log_data += "From: " + result_.second.second.first + "\n";
      log_data += "To: " + result_.second.second.second + "\n###\n";
    }
    if (!results_.empty()) {
      utility::save_log(log_data, target, replacer,
                        this->currentOperation.logFilePath);
    }
  };
  for (auto &code : this->file_instances) {
    for (auto const &result : code.replace(target, replacer)) {
      if ((this->filepaths.find(code.getpath()) != this->filepaths.end() &&
           this->currentOperation.get_operationscope() ==
               operation_scope::some) ||
          this->currentOperation.get_operationscope() ==
              operation_scope::some) {
        results_.push_back({code.getpath(), result});
      }
    }
  }
  print_results();
}

void REX::revert() {
  unordered_map<string, vector<pair<string, string>>> files_;
  code_helper logFile(this->currentOperation.get_targetAndreplacer().first);
  string prev;
  for (auto const &code : logFile.getcode()) {
    if (code.compare("###") == 0) {
      continue;
    }
    size_t index = code.find_first_of(':');
    string s = code;
    if (index != -1) {
      s[index] = '\n';
    }
    auto code_ = utility::split(s + '\n');
    if (code_[0] == "file") {
      string file = utility::split(code_[1], ',')[0];
      utility::trim(file);
      files_[file].push_back({"", ""});
      prev = file;
    } else if (code_[0] == "From") {
      files_[prev].back().first = code_[1];
    } else if (code_[0] == "To") {
      files_[prev].back().second = code_[1];
    } else {
      prev = "*";
    }
  }
  for (auto &file : files_) {
    string f_name = file.first;
    if (f_name[0] == '.') {
      f_name =
          utility::getcwd_string() + string(f_name.begin() + 1, f_name.end());
    }
    code_helper code_file(f_name);
    for (; !file.second.empty(); file.second.pop_back()) {
      string tar = file.second.back().second, rep = file.second.back().first;
      utility::trim(tar);
      utility::trim(rep);
      // cin.get();
      for (auto &code : code_file.getcode()) {
        size_t indx = code.find(tar);
        while (indx != string::npos) {
          string s;
          for (int i = 0; i < code.length(); i++) {
            if (i == indx) {
              s += rep, i += tar.length() - 1;
            } else {
              s.push_back(code[i]);
            }
          }
          code = s;
          // cin.get();
          indx = code.find(tar); // To make sure not even an instance of the
                                 // target is left...
        }
        // cin.get();
      }
    }
    code_file.revert();
  }
  remove(this->currentOperation.get_targetAndreplacer()
             .first); // Removing the logFile so, that it could not be used any
                      // further...
}

int main(int argc, char *argv[]) {
  cout << utility::getcwd_string() << " \n";
  if (argc < 3) {
    REX::printUsage();
    return 0;
  }
  try {
    REX instance(argv, argc);
    instance.run();
  } catch (std::exception &e) {
    cout << "\nERROR: " << e.what() << "\n\n";
    REX::printUsage();
  }
  return cout << "\n", 0;
}
